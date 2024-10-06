#include "Board.h"

Board::Board(int size)
    : m_Size(size)
{
    // reserve enough place for the vector since it's heap allocated (we avoid unnecessary reallocation)
    m_Map.reserve(size * size);

    // fill the map
    for (int i = 0; i < size * size; ++i)
    {
        m_Map.emplace_back(0);
    }
}


void Board::Display() const
{
    std::cout << "  \t";
    for (int i = 0; i < m_Size; ++i)
    {
        Log(i);
        std::cout << " ";
    }
    Log(std::endl);
    for (int i = 0; i < m_Size * m_Size; ++i)
    {
        if (i % 9 == 0)
            std::cout << '\n' << i << '\t';

        if (m_Map[i] == 1)
        {
            APPLY_BLUE(m_Map[i]);
        }
        else if (m_Map[i] == 2)
        {
            APPLY_RED(m_Map[i]);
        }
        else if (m_Map[i] == 3)
        {
            APPLY_GREEN(m_Map[i]);
        }
        else if (m_Map[i] == 4)
        {
            APPLY_YELLOW(m_Map[i]);
        }
        else if (m_Map[i] == -1)
        {
            APPLY_GREY('x');
        }
        else
        {
            Log(m_Map[i]);
        }

        std::cout << " ";
    }
    std::cout << "\n\n" << std::flush;
}


TileType Board::Update(int pos, int player)
{
    m_Map[pos] = player;
    return TileType::Free;
}


bool Board::IsFinish() const
{
    if (std::find(m_Map.begin(), m_Map.end(), 0) != std::end(m_Map))
    {
        // we found a 0
        return false;
    }
    std::cout << "Finish !!!" << std::endl;
    return true;
}