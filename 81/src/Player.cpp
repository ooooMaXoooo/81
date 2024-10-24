#include "Player.h"

#include "data_structures/Pile.h"
#include "data_structures/File.h"
#include "utils.h"

#include <assert.h>

std::vector<int> Player::m_PlayedTiles;




Player::Player(std::shared_ptr<Board> board)
    : m_Board(board), m_AreaScore(0)
{
    m_PlayedTiles.reserve(41);
    m_NbPlayers++;

    m_ID = m_NbPlayers;

    m_AreasStorage.fill(false);
}

Player::~Player()
{
    m_NbPlayers--;
}

void Player::SendGreyTiles()
{
    //compute grey tiles
    UpdateGreyTiles();
    //send data to the board
    for (int i = 0; i < m_GreyTiles.size(); ++i)
    {
        m_Board->Update(m_GreyTiles[i], -1);
    }
}

void Player::ClearGreyTiles()
{
    const size_t size = m_GreyTiles.size();
    for (int i = 0; i < size; ++i)
    {
        m_Board->Update(m_GreyTiles[0], 0);
        m_GreyTiles.erase(m_GreyTiles.begin());
    }
}

void Player::Play()
{
    return;
    /*

    int pos = -1;
    switch (m_Type)
    {
    case PlayerType::Human:
        pos = PlayAt_Human();
        break;

    case PlayerType::Bot:
        pos = PlayAt_Bot(m_Board->GetMap());
        break;

    default:
        break;
    }

    if (m_Board->IsFinish())
        return;

    m_LastPos = pos;
    m_PlayedTiles.emplace_back(pos);
    m_Board->Update(pos, m_ID);


    Update_PositionScore_map(pos);
    Update_AreaScore(pos);*/
}

float Player::score()
{
    return m_AreaScore * 0.5 + LinearScore();
}

void Player::UpdateGreyTiles()
{
    int boardSize = m_Board->Size();

    // right side
    if ((m_LastPos + 1) % 9 == 0)
    {
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);
        m_GreyTiles.emplace_back(m_LastPos - boardSize - 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize - 1);
    }
    // left side
    else if (m_LastPos % 9 == 0)
    {
        m_GreyTiles.emplace_back(m_LastPos + 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);
        m_GreyTiles.emplace_back(m_LastPos - boardSize + 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize + 1);
    }
    // all pos are in [0; 80] --> don't need to check if it's greater than 0
    // bottom row
    else if (m_LastPos < boardSize) // maybe * 3 for size
    {
        m_GreyTiles.emplace_back(m_LastPos + 1);
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize + 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize - 1);
    }
    // top row
    else if (m_LastPos > (boardSize * (boardSize - 1)))
    {
        m_GreyTiles.emplace_back(m_LastPos + 1);
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos - boardSize + 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize - 1);
    }
    // midle
    else
    {
        m_GreyTiles.emplace_back(m_LastPos - 1);
        m_GreyTiles.emplace_back(m_LastPos + 1);

        m_GreyTiles.emplace_back(m_LastPos - boardSize);
        m_GreyTiles.emplace_back(m_LastPos + boardSize);

        m_GreyTiles.emplace_back(m_LastPos - boardSize - 1);
        m_GreyTiles.emplace_back(m_LastPos - boardSize + 1);

        m_GreyTiles.emplace_back(m_LastPos + boardSize - 1);
        m_GreyTiles.emplace_back(m_LastPos + boardSize + 1);
    }

    // check for played tile
    int count = 0;
    const size_t size = m_GreyTiles.size();
    for (int i = 0; i < size; ++i)
    {
        if ((std::find(m_PlayedTiles.begin(), m_PlayedTiles.end(), m_GreyTiles[count]) != std::end(m_PlayedTiles))
            || m_GreyTiles[count] < 0
            || m_GreyTiles[count] > 80)
        {
            m_GreyTiles.erase(m_GreyTiles.begin() + count);
            continue;
        }
        ++count;
    }
}

void Player::Update_PositionScore_map(uint8_t pos) {
    // look for neighbors of the position of the player
    std::vector<uint8_t> neighbors;

    neighbors.reserve(4);
    getNeighbors(neighbors, pos);
    // now all neighbors are valid and belong to different chain
    
    const uint8_t SIZE = neighbors.size();

    uint8_t main_head_pos;

    switch (SIZE) {

        // if there is 1 neighbor, we add the current position to the chain and we update the score
        case 1:
            main_head_pos = findHead(neighbors[0]);


            m_PosScore_map[main_head_pos]->score += 1;
            m_PosScore_map[pos] = m_PosScore_map[main_head_pos];
            /*std::cout << "1 neighbor : Adding to the branch : " << (int)m_PosScore_map[main_head_pos]->head_pos << std::endl;
            std::cout << "\t : score of the branch : " << (int)m_PosScore_map[main_head_pos]->score << std::endl;

            for (const auto& [pos, head_score] : m_PosScore_map)
            {
                const auto& [head, score] = *head_score;

                std::cout << "\n\nPosition :\n\t" << (int)pos << '\n';
                std::cout << "Head position :\n\t" << (int)head << '\n';
                std::cout << "Score :\n\t" << (int)score << std::endl << std::endl;
            }*/

            break;

        // if there is 0, we create a new chain with a score of 1
        case 0:
            m_PosScore_map[pos] = std::make_shared<Score_Head>(pos, 1);
            //std::cout << "0 neighbor : Creating a new branch : " << (int)m_PosScore_map[pos]->head_pos << std::endl;
            break;


        // if there is more than 2, we merge chains
        default:
            main_head_pos = findHead(neighbors[0]);

            //std::cout << "2+ neighbors : Merging branches: " << std::endl;
            // we handle the case with the same chain in the getNeighbor function
            mergeChains(neighbors);
            m_PosScore_map[pos] = m_PosScore_map[main_head_pos];


            /*for (const auto& [pos, head_score] : m_PosScore_map)
            {
                const auto& [head, score] = *head_score;

                std::cout << "\n\nPosition :\n\t" << (int)pos << '\n';
                std::cout << "Head position :\n\t" << (int)head << '\n';
                std::cout << "Score :\n\t" << (int)score << std::endl << std::endl;
            }*/

            break;
        }

}

void Player::getNeighbors(std::vector<uint8_t>& neighbors, uint8_t pos)
{
    const int BOARD_SIZE = m_Board->Size();
    std::vector<int>& board = m_Board->GetMap();

    // right side
    if ((pos + 1) % BOARD_SIZE == 0)
    {
        neighbors.emplace_back(pos - 1);
        neighbors.emplace_back(pos - BOARD_SIZE);
        neighbors.emplace_back(pos + BOARD_SIZE);
    }
    // left side
    else if (pos % 9 == 0)
    {
        neighbors.emplace_back(pos - BOARD_SIZE);
        neighbors.emplace_back(pos + 1);
        neighbors.emplace_back(pos + BOARD_SIZE);
    }
    // all pos are in [0; 80] --> don't need to check if it's greater than 0
    /*// bottom row
    else if (pos < BOARD_SIZE) // maybe * 3 for size
    {
        neighbors.emplace_back(pos + 1);
        neighbors.emplace_back(pos - 1);
        neighbors.emplace_back(pos + BOARD_SIZE);
    }
    // top row
    else if (pos > (BOARD_SIZE * (BOARD_SIZE - 1)))
    {
        neighbors.emplace_back(pos + 1);
        neighbors.emplace_back(pos - 1);
        neighbors.emplace_back(pos - BOARD_SIZE);
    }*/
    // midle
    else
    {
        neighbors.emplace_back(pos - 1);
        neighbors.emplace_back(pos - BOARD_SIZE);
        neighbors.emplace_back(pos + 1);
        neighbors.emplace_back(pos + BOARD_SIZE);
    }

    // keep only our own neighbors
    int count = 0;
    const uint8_t SIZE = neighbors.size();

    // a table to put all chains' head
    std::vector<uint8_t> head_map;
    head_map.reserve(4);

    for (int i = 0; i < SIZE; ++i)
    {
        // we check if the position is valid
        if (neighbors[count] < 0 || neighbors[count] > 80)
        {    
            neighbors.erase(neighbors.begin() + count);
            continue;
        }
        // we verify that the slot belongs to the player
        if (board[neighbors[count]] != m_ID)
        {
            neighbors.erase(neighbors.begin() + count);
            continue;
        }
        // we remove a neighbors if they share the same chain
        uint8_t main_head_pos = findHead(neighbors[count]);

        if (std::find(head_map.begin(), head_map.end(), main_head_pos) != head_map.end())
        {
            neighbors.erase(neighbors.begin() + count);
            continue;
        }

        head_map.emplace_back(m_PosScore_map[neighbors[count]]->head_pos);
        ++count;
    }
}

void Player::mergeChains(const std::vector<uint8_t>& neighbors)
{
    // the head's position of the first chain
    uint8_t main_head_pos = findHead(neighbors[0]);

    // we start at the second neighbor to merge all them into the first one
    // we know that we have at least 2 neighbors in the vector

    const uint8_t SIZE = neighbors.size();
    for (int i = 1; i < SIZE; i++)
    {
        m_PosScore_map[main_head_pos]->score += m_PosScore_map[neighbors[i]]->score;
        m_PosScore_map[neighbors[i]]->head_pos = main_head_pos;
        /*std::cout << "Merging chain " << (int)m_PosScore_map[neighbors[i]]->head_pos <<
            "in " << (int)m_PosScore_map[main_head_pos]->score << '\n' << std::endl;*/
    }

    m_PosScore_map[main_head_pos]->score++;
}


uint8_t Player::LinearScore() const
{
    uint8_t max_score = 0;

    for (auto& [pos, head_score] : m_PosScore_map)
    {
        auto& [head, score] = *head_score;

        max_score = utils::max(max_score, score);
    }

    return max_score;
}














void Player::Update_AreaScore(uint8_t pos)
{
    uint8_t cell_row, cell_col;

    // get the cell in which the player played
    GetPosCell(pos, cell_row, cell_col);


    // look if the player own the cell
    bool isOwning = OwnCell(cell_row, cell_col);

    if (isOwning)
    {
        m_AreaScore += 9;
        m_AreasStorage[cell_row * 3 + cell_col] = true;
    }

}


bool Player::OwnCell(uint8_t cell_row, uint8_t cell_col) const
{
    // store how many players own a slot in the cell
    std::unordered_map<uint8_t, uint8_t> slots_count;

    // the board on which we play
    std::vector<int>& board = m_Board->GetMap();

    // an offset to look at the good positions. i.e. the right cell
    uint8_t offset = cell_row * 27 + cell_col * 3;

    // all positions of the first cell
    const uint8_t firstCellPos[9] = {
         0,   1,   2,
         9,  10,  11,
        18,  19,  20
    };


    // look through each pos for the given cell
    for (auto position : firstCellPos)
    {
        uint8_t owner = board[position + offset];
        // removing blocked Tiles from our score since it doesn't matter to us
        if (owner == -1)
            continue;

        slots_count[owner] = slots_count.contains(owner) ? slots_count.at(owner) + 1 : 1;
    }


    /*                  ALGORITHM
    * 
    *   • There is from 2 to 9 players 
    *
    *   
    *   • We have differents notations:
    *       • "n"  is the number of slots owned by the player who owns the most slots in the cell
    *       • "n'" is the number of slots owned by the player who owns the most slots in the cell after the first one
    *       • "F" is the number of remaining free slots in the cell
    *       • "n'_id" is the player who owns the most slots in the cell after the first one.
    *               We make it 10 if two players are at this rank
    * 
    * 
    *   - Algorithm :
    * 
    *       • if we previously own the cell, return false to don't add "infinitly" points
    *       
    *       • if the best player is "0" (i.e. free slot)
    *           • we don't know the cell's owner
    * 
    *       (compute n here)
    *       (compute n' here)
    *       (compute n'_id here)
    * 
    *       • Else if the cell is full (F = 0)  --->  don't need F since we just check if it exist in the map
    *           • The player who owns the most cell is the owner of the cell.
    *       
    *       • Else if 5 <= n  (n > 4 for computations)
    *           • The player who owns the most cell is the owner of the cell.
    * 
    *       • Else
    *           (compute F here)   ---> we just need to look in the map since we know it exist
    *           • if n = 2
    *               • We don't know the cell's owner
    * 
    *           • Else if F = n-1
    *               • If n = 4
    *                   • We don't know the cell's owner
    *               • Else (n = 3)
    *                   • If n'_id = 0
    *                       • The player who owns the most cell is the owner of the cell.
    *                   • Else
    *                       • We don't know the cell's owner
    * 
    *           • Else if F = 1
    *               • If n' > n-2
    *                   • We don't know the cell's owner
    *               • Else
    *                   • The player who owns the most cell is the owner of the cell.
    *           
    *           • Else ( ==> F = 2)
    *               • If n' < n-2
    *                   • The player who owns the most cell is the owner of the cell.
    *               • Else
    *                   • We don't know the cell's owner
    * 
    */

    if (m_AreasStorage[cell_col + cell_row * 3])
        return false;


    // we count which player have the most slot in the cell as well as the second one
    int16_t best = -1, secondBest = -1;
    int16_t bestID = -1, secondBestID = -1;
    for (const auto& [id, count] : slots_count)
    {
        if (count == best)
            bestID = 0;
        else if (count > best)
        {
            bestID = id;
            best = count;
        }
        else //  count < best
        {
            if (count == secondBest)
            {
                // in the case where there is two 2nd best, we ignore "0"
                if (id == 0)
                    continue;
                secondBestID = 10;
            }
            else if (count > secondBest)
            {
                secondBest = count;
                secondBestID = id;
            }
        }
    }


    // full cell --> we know the owner
    if (!slots_count.contains(0))
    {
        return bestID == m_ID;
    }

    // if n >= 5  --> we know the owner
    if (best > 4)
        return bestID == m_ID;



    uint8_t freeSlots = slots_count.at(0);
    
    // if n = 2 --> we don't know the owner
    if (best == 2)
        return false;

    // when there is n-1 free slots
    if (freeSlots == best - 1)
    {
        if (best == 4)
            return false;

        if (secondBestID == 0)
            return bestID == m_ID;

        return false;
    }

    // when there is only one free slots
    if (freeSlots == 1)
    {
        // if n' > n-2  --> we don't know the owner
        if (secondBest > best - 2)
            return false;

        return bestID == m_ID;
    }

    // we know that freeSlots = 2
    // if n' < n-2  -->  we know the owner
    if (secondBest < best - 2)
        return bestID == m_ID;

    return false;
}

void Player::GetPosCell(uint8_t pos, uint8_t& row, uint8_t& col) const
{
    const int size = m_Board->GetMap().size();

    const uint8_t SQRT_SIZE = sqrt(size);

    // the row and the column of the position in the board
    int row_pos{};
    int column_pos{};

    // the row and the column in the cell of the position in the board
    int relative_row{};
    int relative_column{};

    // the position in the cell based on the relative row and column
    int relative_pos{};

    // the id of the the cell in which the position is.
    int cell_num{};




    // convert each position in 2 dimensional board
    row_pos = pos / SQRT_SIZE;
    column_pos = (pos - row_pos * SQRT_SIZE);

    // convert the row and the column relative to a cell
    relative_row = row_pos % 3;
    relative_column = column_pos % 3;

    // convert the relative row and column into a single position in a flatten-cell
    relative_pos = relative_row * 3 + relative_column;


    // we make all indices/positions to point to the first slot in their cell.
    cell_num = pos - ((relative_pos * 3) - 2 * (relative_pos % 3));
    /*  0    3   6
        *  27  30  33
        *  54  57  60
    */

    cell_num /= 3;
    /*  0    1    3
        *   9   10   11
        *  18   19   20
    */

    cell_num -= (pos / 27) * 6;
    /*    0   1   2
        *   3   4   5
        *   6   7   8
        */

    row = cell_num / 3;
    col = cell_num % 3;
}

uint8_t Player::findHead(uint8_t pos) const
{
    assert(m_PosScore_map.contains(pos));

    uint8_t main_head_pos = m_PosScore_map.at(pos)->head_pos;

    while (m_PosScore_map.at(main_head_pos)->head_pos != main_head_pos)
        main_head_pos = m_PosScore_map.at(main_head_pos)->head_pos;

    return main_head_pos;
}

void Player::convertPosNumber_to_Char(uint8_t pos, std::string* str_out) const
{
    (*str_out)[0] = 'A' + pos / 9;
    (*str_out)[1] = '1' + pos % 9;
}
