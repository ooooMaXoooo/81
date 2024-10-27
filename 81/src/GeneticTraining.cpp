#include "GeneticTraining.h"
#include <string>

#include <utility>
#include "utils.h"
#include <algorithm>


#include <random>

namespace AI
{
	void GeneticTraining::train(const char* file_to_recover, const char* saveFile_path, int savePeriod)
	{

		const char* formats[8] =
		{
			"9BBBBBBBBB",
			"8BBBBBBBB",
			"7BBBBBBB",
			"6BBBBBB",
			"5BBBBB",
			"4BBBB",
			"3BBB",
			"2BB"
		};


		// creates games
		for (int i = 0; i < m_nbGames; i++)
		{
			m_Games.emplace_back(std::make_unique<Game>(formats[i % 8], file_to_recover));
		}




		for (int gen = 0; gen < m_maxGen; gen++)
		{
			std::cout << "********************\nGen " << gen << " started !" << std::endl;

			// built the selected bots vector
			std::vector<std::pair<NeuralBot, float>> selection;
			selection.reserve(m_nbGames);

			for (int gameIndice = 0; gameIndice < m_nbGames; gameIndice++)
			{
				//play each games
				//std::cout << "\nGame " << gameIndice <<" Started !!!\n";
				//std::cin.get();
				int count = 0;
				while (!m_Games[gameIndice]->ShouldClose())
				{
					count++;
					m_Games[gameIndice]->simulation_step();
					//std::cout << "\rStep " << count++;
				}
				//std::cout << "\nGame "<< gameIndice << " Finished !!!\nEvaluating players !\n";
				std::vector<float> players_score = m_Games[gameIndice]->scores();

				std::vector<float> players_fitness;
				const int _SIZE = static_cast<int>(players_score.size());

				players_fitness.reserve(_SIZE);

				// evaluate every players on a single game
				for (int player_indice = 0; player_indice < _SIZE; player_indice++)
				{
					bool has_won = false;
					float best_score = 0;
					float second_best_score = 0;
					const float p_score = players_score[player_indice];

					utils::maxAndSecondMax(&players_score[0], static_cast<int>(players_score.size()), best_score, second_best_score);

					has_won = best_score == p_score && std::count(players_score.begin(), players_score.end(), p_score) < 2;

					const float fit = fitness(has_won, p_score, best_score, second_best_score);
					players_fitness.emplace_back(fit);
					//std::cout << "\n\t\t##### Bot " << player_indice + 1 << " fitness : " << fit << std::endl;
				}

				// get the best player and insert it in the selection array
				const auto max_fit_indice = std::max_element(players_fitness.begin(), players_fitness.end());
				const int bestPlayerIndice = static_cast<int>(std::distance(players_fitness.begin(), max_fit_indice));

				selection.emplace_back(std::make_pair(m_Games[gameIndice]->getBot(bestPlayerIndice), *max_fit_indice));
				//std::cout << "\nBot " << m_Games[gameIndice]->getBot(bestPlayerIndice).id() << " is chosen on game " << gameIndice
				//	<< "\tGen " << gen << "\n\n";
				//selection_fitness.emplace_back(*max_fit_indice);
			}
			std::cout << "Finished building the selection\n";

			// we have our selection
			// make them reproduce
			// shuffle the array to have more randomness
			std::random_device rd;
			std::mt19937 g(rd());

			std::shuffle(selection.begin(), selection.end(), g);

			for (int i = 0; i < m_nbGames / 2; i++)
			{
				const auto [child1, child2] = reproduce(selection[2 * i].first, selection[2 * i + 1].first);

				selection[2 * i].first = child1;
				selection[2 * i + 1].first = child2;
			}

			// suffle evreything again
			std::shuffle(selection.begin(), selection.end(), g);

			// mutations
			std::vector<NeuralBot> population;
			population.reserve(m_nbGames);

			std::vector<float> fitness;
			fitness.reserve(m_nbGames);

			for (auto& [bot, fit] : selection)
			{
				population.emplace_back(bot);
				fitness.emplace_back(fit);
			}


			if (gen % savePeriod == savePeriod - 1)
			{
				const auto max_fit_indice = std::max_element(fitness.begin(), fitness.end());
				const int bestPlayerIndice = static_cast<int>(std::distance(fitness.begin(), max_fit_indice));

				save(saveFile_path, *population[bestPlayerIndice].getPtr_Ntw());
			}

			mutations(population);

			// reset games
			//clean up
			resetGames();

			// populate games with the new selection
			populate(population);

			std::cout << "Gen " << gen << " finished !" << "\n********************\n" << std::endl;
		}

		std::cout << "\n\nTraining finish !" << std::endl;
	}

	void GeneticTraining::save(std::string filepath, NeuralNetwork nt)
	{
		filepath.append("_" + std::to_string(nbSaves) + ".ntw");
		nbSaves++;

		nt.saveConfig(filepath.c_str());
	}




	float GeneticTraining::fitness(bool has_won, float personnal_score, float best_score, float second_best_score)
	{
		float winPoints = has_won ? 1.0f : -1.0f;

		// delta = p_score - bestScore + p_score - s_bestScore
		float delta = 2 * personnal_score - best_score - second_best_score;


		// apply a sigmoid
		delta = 100 * (2 / (1 + exp(-0.1f * delta)) - 1);


		// if we don't win even with the best score, we don't add points

		return 250 * winPoints + delta;
	}


	std::array<NeuralBot, 2> GeneticTraining::reproduce(NeuralBot& bot1, NeuralBot& bot2)
	{
		NeuralBot b1(bot1), b2(bot2);


		NeuralNetwork* p_ntw1 = bot1.getPtr_Ntw();
		NeuralNetwork* p_ntw2 = bot2.getPtr_Ntw();

		std::vector<uint> HL_dim_1 = p_ntw1->getHiddenLayersDimension();
		std::vector<uint> HL_dim_2 = p_ntw2->getHiddenLayersDimension();

		uint max1_size, max2_size;

		max1_size = static_cast<uint>(HL_dim_1.size());
		max2_size = static_cast<uint>(HL_dim_2.size());

		const bool max1Greater = max1_size > max2_size;
		const uint MIN_HL_DIM = max1Greater ? max2_size : max1_size;
		const uint MAX_HL_DIM = max1Greater ? max1_size : max2_size;

		const uint min_max[2] = { MIN_HL_DIM, MAX_HL_DIM };


		for (uint layer_indice = 0; layer_indice < MIN_HL_DIM; layer_indice++)
		{
			// determinate the minimum numbers of neurons that we have in a layer
			const uint min_neuron = HL_dim_1[layer_indice] < HL_dim_2[layer_indice] ? HL_dim_1[layer_indice] : HL_dim_2[layer_indice];

			const uint cross_indice = std::rand() % min_neuron;
			for (uint neuron_indice = cross_indice; neuron_indice < min_neuron; neuron_indice++)
			{
				const Neuron temp_neuron = b1.getPtr_Ntw()->getNeuron(layer_indice, neuron_indice);

				// differents cases for wethere a neuron have more or less connections is handle in the function changeNeuron
				b1.getPtr_Ntw()->changeNeuron(b2.getPtr_Ntw()->getNeuron(layer_indice, neuron_indice - cross_indice), layer_indice, neuron_indice);
				b2.getPtr_Ntw()->changeNeuron(temp_neuron, layer_indice, neuron_indice - cross_indice);
			}

			// don't need to act on extra neuron since they are already in place
		}

		// don't need to act on extra layers since they are already in place
		return std::array<NeuralBot, 2>{b1, b2};
	}

	void GeneticTraining::resetGames()
	{
		for (int i = 0; i < m_nbGames; i++)
		{
			m_Games[i]->restart();
		}
	}


	void GeneticTraining::mutations(std::vector<NeuralBot>& selection)
	{
		for (int i = 0; i < selection.size(); i++)
		{
			selection[i].mutate();
		}
	}

	void GeneticTraining::populate(std::vector<NeuralBot>& selection)
	{
		for (int gameIndice = 0; gameIndice < m_nbGames; gameIndice++)
		{
			// get the number of player in the game
			const uint8_t nbPlayer = m_Games[gameIndice]->nbPlayers();

			// we make sure that there is at least every bot in the selection in the game:
			selection[gameIndice].setNumberPlayers(nbPlayer);
			
			m_Games[gameIndice]->changeBot(0, selection[gameIndice]);


			// generate nbPlayer - 1 player and put them into the game
			for (int i = 1; i < nbPlayer; i++)
			{
				const uint random = std::rand() % m_nbGames; // m_nbGames == selection.size();
				selection[random].setNumberPlayers(nbPlayer);

				m_Games[gameIndice]->changeBot(i, selection[random]);
			}
		}
	}
}