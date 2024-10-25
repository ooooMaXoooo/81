#pragma once

#include "Game.h"
#include <math.h>


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
		void save();

		void nextGen();


		float fitness(bool has_won, int personnal_score, int best_score);
		NeuralBot selection_function(const Game& game);

		void newGeneration();
	};

}
