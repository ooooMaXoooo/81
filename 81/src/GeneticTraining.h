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

		int nbSaves;

	public :
		GeneticTraining(int maxGen = 500, int nbGames = 200, int startnbSaves = 0)
			: m_maxGen(maxGen), m_nbGames(nbGames), nbSaves(startnbSaves)
		{
			m_Games.reserve(nbGames);
		}

		void train(const char* file_to_recover = "\0null", const char* saveFile_path = "./res/NTW/neural_bot", int savePeriod = 50);

	private :
		void save(std::string filepath, NeuralNetwork nt);

		//void nextGen(std::vector<NeuralBot> playerPool);


		float fitness(bool has_won, float personnal_score, float best_score, float second_best_score);

		std::array<NeuralBot, 2> reproduce(NeuralBot& bot1, NeuralBot& bot2);

		void resetGames();

		void mutations(std::vector<NeuralBot>& selection);
		void populate(std::vector<NeuralBot>& selection);
	};

}
