#include "GeneticTraining.h"
#include <string>

#include <utility>

namespace AI
{
	void GeneticTraining::train(const char* saveFile_path, int savePeriod, int elitism)
	{
		const char* formats[7] =
		{
			"9BBBBBBBBB",
			"8BBBBBBBB",
			"7BBBBBBB",
			"6BBBBBB"
			"5BBBBB",
			"4BBBB",
			"3BBB",
			"2BB"
		};

		// creates games
		for (int i = 0; i < m_nbGames; i++)
		{
			m_Games.emplace_back(std::make_unique<Game>(formats[i%7]));
		}



		// built the selected bots vector
		std::vector<NeuralBot> selection;

		for (int gameIndice = 0; gameIndice < m_nbGames; gameIndice++)
		{
			//play each games
			while (!m_Games[gameIndice]->ShouldClose())
			{
				m_Games[gameIndice]->simulation_step();
			}
			// evaluate each player
			std::vector<uint8_t> players_score = m_Games[gameIndice]->scores();

			std::vector<float> players_fitness;
			const int _SIZE = players_fitness.size();

			players_fitness.reserve(_SIZE);

			for (int player_indice = 0; player_indice < _SIZE; player_indice++)
			{
				bool has_won = false;
				float best_score = 0;
				const float p_score = players_score[player_indice];

				best_score = *std::max_element(players_score.begin(), players_score.end());

				has_won = best_score == p_score && std::count(players_score.begin(), players_score.end(), p_score) < 2;

				players_fitness.emplace_back(fitness(has_won, p_score, best_score));
			}


		}

	}
}