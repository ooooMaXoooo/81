#pragma once

#include <iostream>
#include <vector>


#define Log(x) std::cout << x

#define APPLY_BLUE(x) Log("\033[38;5;81m"); Log(x); Log("\033[0m")

#define APPLY_RED(x) Log("\033[38;5;196m"); Log(x); Log("\033[0m")

#define APPLY_YELLOW(x) Log("\033[38;5;226m"); Log(x); Log("\033[0m")

#define APPLY_GREEN(x) Log("\033[38;5;118m"); Log(x); Log("\033[0m")

#define APPLY_GREY(x) Log("\033[38;5;13m"); Log(x); Log("\033[0m")

        // "\033[38;5;{ID}m" foreground color
        // "\033[48;5;{ID}m" background color

        // ID : (see ANSI table)
        // 196 red
        // 81 blue
        // 226 yellow
        // 118 green
        // 8 grey


enum TileType
{
    GreyTile = -1,
    Free = 0,
    PlayedTile = 1
};

class Board
{
private:
    // the size of the board <--> the square root of the number of tiles
    const int m_Size;

    // the vector that contains each tiles.
    //-1 for grey tile //
    // 0 for unused  //
    // 1 for player 1 //
    //           ...            //
    // x for player x //
    std::vector<int> m_Map;

public:
    /*
    * Create an instance of a Board
    * @param size : the length of the board's side
    */
    Board(int size = 9);

    // "\033[38;5;{ID}m" foreground color
    // "\033[48;5;{ID}m" background color

    // ID : (see ANSI table)
    // 196 red
    // 81 blue
    // 226 yellow
    // 118 green


    void Display() const;

    TileType Update(int pos, int player);

    bool IsFinish() const;


    std::vector<int>& GetMap() { return m_Map; }
    int Size() const { return m_Size; }
};