#pragma once

#include "Game.h"
#include <math.h>
#include "NeuralBot.h"
#include <memory>

namespace AI
{
	class GeneticTraining
	{

		std::vector<std::unique_ptr<Game>> m_Games;

		// we start at gen = 0
		int m_maxGen;

		int m_nbGames;

	public :
		GeneticTraining(int maxGen = 500, int nbGames = 200)
			: m_maxGen(500), m_nbGames(nbGames)
		{
			m_Games.reserve(nbGames);
		}

		void train(const char* saveFile_path = "./res/NTW/neural_bot", int savePeriod = 50, int elitism = 5);

	private :
		void save(const char* filepath);

		//void nextGen(std::vector<NeuralBot> playerPool);


		float fitness(bool has_won, int personnal_score, int best_score, int second_best_score);

		std::array<NeuralBot, 2> reproduce(NeuralBot& bot1, NeuralBot& bot2);

		void mutations(std::vector<NeuralBot>& selection);
	};

}
