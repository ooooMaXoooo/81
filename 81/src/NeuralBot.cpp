#include "NeuralBot.h"

#include <random>


NeuralBot::NeuralBot(std::shared_ptr<Board> board, uint16_t id, uint8_t nbPlayers)
    : Player {board, id},
    m_ntw(NeuralNetwork(81 + 1  + 1, 81, { 50, 50, 50 })),
    m_nbPlayer(nbPlayers)
{
}

NeuralBot::NeuralBot(const NeuralBot& neuralBot)
    : m_ntw(neuralBot.m_ntw), m_nbPlayer(neuralBot.m_nbPlayer), 
    Player{ neuralBot.m_Board, neuralBot.m_ID }
{
}

void NeuralBot::Play()
{
    uint8_t pos = -1;

    // 4 entries : 
    //  the map
    //  the turn --> don't need
    //  the player id
    //  number of player
    std::vector<uint> entries = m_Board->GetMap();
    entries.reserve(entries.capacity() + 2);

    entries.emplace_back(m_ID);
    entries.emplace_back(m_nbPlayer);

    std::vector<float> proba = m_ntw.output(entries);

    std::vector<std::pair<float, uint>> prob_pos;

    const uint _SIZE = proba.size();

    prob_pos.reserve(_SIZE);

    for (int i = 0; i < _SIZE; i++)
    {
        prob_pos.emplace_back(std::make_pair(proba[i], i));
    }

    std::sort(prob_pos.begin(), prob_pos.end());

    // from the most highest chance to the least one
    for (uint i = _SIZE - 1; i > -1; i--)
    {
        const uint position = prob_pos[i].second;
        if (position == m_LastPos
            || std::find(m_GreyTiles.begin(), m_GreyTiles.end(), position) != m_GreyTiles.end()
            || m_Board->GetMap()[position] != 0)
        {
            continue;
        }

        pos = position;
        break;
    }
   
    std::cout << "Bot " << m_ID << " plays at " << (int)pos << std::endl;

    m_LastPos = pos;
    m_PlayedTiles.emplace_back(pos);
    m_Board->Update(pos, m_ID);
         
         
    Update_PositionScore_map(pos);
    Update_AreaScore(pos);
}

void NeuralBot::mutate()
{
    /* list of possible mutations
    * 
    *       • change a weight       --->    50%
    *       • add a neuron          --->    20%
    *       • remove a neuron       --->    10%
    */

    bool change_weight = std::rand() % 2;
    bool add_neuron    = !(std::rand() % 5);
    bool remove_neuron = !(std::rand() % 10);


    if (change_weight)
    {
        const uint nbHL = m_ntw.getNbHiddenlayers();
        
        const uint layer = std::rand() % (nbHL + 1);

        uint neuronPos{}, weightPos{};


        if (layer == nbHL)
        {
            neuronPos = std::rand() % m_ntw.getHiddenLayersDimension()[nbHL - 1];

            weightPos = std::rand() % m_ntw.getNbOutputs();
        }
        else if (layer == 0)
        {
            neuronPos = std::rand() % m_ntw.getNbEntries();

            weightPos = std::rand() % m_ntw.getHiddenLayersDimension()[0];
        }
        else
        {
            neuronPos = std::rand() % m_ntw.getHiddenLayersDimension()[layer - 1];

            weightPos = std::rand() % m_ntw.getHiddenLayersDimension()[layer];
        }

        float value = std::rand() / static_cast <float> (RAND_MAX);

        m_ntw.changeWeight(layer, neuronPos, weightPos, m_ntw.getWeight(layer, neuronPos, weightPos) + value);
    }


    if (add_neuron)
    {
        const uint layer = std::rand() % (m_ntw.getNbHiddenlayers());

        m_ntw.addNeuron(layer);
    }
    else if (remove_neuron)
    {
        const uint nbHL = m_ntw.getNbHiddenlayers();

        const uint layer = std::rand() % nbHL;

        uint neuronPos{};


        neuronPos = std::rand() % m_ntw.getHiddenLayersDimension()[layer];

        m_ntw.removeNeuron(layer, neuronPos);
    }
}
