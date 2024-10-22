#include "Player.h"

#include <string>

#include "data_structures/Pile.h"
#include "data_structures/File.h"
#include "utils.h"


std::vector<int> Player::m_PlayedTiles;




Player::Player(int id, PlayerType type, std::shared_ptr<Board> board)
    : m_ID(id), m_Type(type), m_Board(board)
{
    m_PlayedTiles.reserve(41);
    m_NbPlayers++;
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

bool Player::Play()
{
    // update greyTile here and not in the main game loop ?

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

    Update_PositionScore_map(pos);
    Update_AreaScore(pos);

    m_LastPos = pos;
    m_PlayedTiles.emplace_back(pos);
    // return if there is an error (in future)
    // TODO : handle potential errors
    m_Board->Update(pos, m_ID);
    return true;
}

float Player::score()
{
    return m_AreaScore + LinearScore();
}

int Player::PlayAt_Bot(const std::vector<int>& map)
{
    return 0;
}

int Player::PlayAt_Human()
{
    std::vector<int>& board = m_Board->GetMap();
    int pos = -1;
    std::string input = "";
    bool correct = false;
    do
    {
        pos = -1;
        pos = InputInNumber(input); // --> modify the string, it's passed by reference

        // verifiy if the tile has already been played
        // check for the last pos
        bool checkPos = pos > -1 && pos < 81;
        correct = checkPos;
        correct = correct && find(m_GreyTiles.begin(), m_GreyTiles.end(), pos) == m_GreyTiles.end();

        if (checkPos)
        {
            correct = correct && board[pos] == 0;
        }


        if (!correct)
            std::cout << "\nErreur impossile de jouer a " << pos << std::endl;

    } while (!correct);

    return pos;
}

int Player::InputInNumber(std::string& input) const
{
    int pos = -1;

    // check that the first character is between A and I
    //      • transform this character in a number between 0 and 8

    // check that the second character is between 1 and 9
    //      • the ascii number for '0' is 48
    //      • lower the number by one so the character will be in [0,8]



    std::cout << "[player " << m_ID << "] : plays at : ";
    std::cin >> input;

    while ( input.size() != 2
         || (input[0] < 'A' || input[0] > 'A' + 8)
         || (input[1] < '1' || input[1] > '9'))
    {
        std::cout << "\n\t***** Wrong format (letter_number,  i.e. B8)*****\n" << std::endl;
        std::cout << "[player " << m_ID << "] : plays at : ";
        std::cin >> input;
    }

    pos = (int)(input[0] - 'A') * 9 + (int)(input[1] - '1');

    std::cout << "\n\n";
    return pos;
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
    // now all valid neighbors belongs to different chain
    
    const uint8_t SIZE = neighbors.size();

    switch (SIZE) {

    // if there is 1 neighbor, we add the current position to the chain and we update the score
    case 1:
        // we are sure that all head_pos are the same for one big chain
        m_PosScore_map[ m_PosScore_map[neighbors[0]]->head_pos ]->score += 1;
        m_PosScore_map[pos] = m_PosScore_map[neighbors[0]];
        std::cout << "1 neighbor : Adding to the branch : " << (int)m_PosScore_map[neighbors[0]]->head_pos << std::endl;
        break;

    // if there is 0, we create a new chain with a score of 1
    case 0:
        m_PosScore_map[pos] = std::make_shared<Score_Head>(pos, 1);
        std::cout << "0 neighbor : Creating a new branch : " << (int)m_PosScore_map[pos]->head_pos << std::endl;
        break;

    // if there is more than 2, we merge chains
    default:
        std::cout << "2 neighbors : Merging branches: " << std::endl;
        // we handle the case with the same chain in the merge function
        mergeChains(neighbors);
        m_PosScore_map[pos] = m_PosScore_map[neighbors[0]];
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
        if (std::find(head_map.begin(), head_map.end(), m_PosScore_map[neighbors[count]]->head_pos) != head_map.end())
        {
            neighbors.erase(neighbors.begin() + count);
            continue;
        }

        m_PosScore_map[neighbors[count]]->head_pos;
        head_map.emplace_back(m_PosScore_map[neighbors[count]]->head_pos);
        ++count;
    }
}

void Player::mergeChains(const std::vector<uint8_t>& neighbors)
{
    // the head's position of the first chain
    const uint8_t _MAIN_HEAD_POS = m_PosScore_map[neighbors[0]]->head_pos;
    /*while (m_PosScore_map[main_head_pos]->head_pos != main_head_pos)
        main_head_pos = m_PosScore_map[main_head_pos]->head_pos;*/


    // if there are no mistake, all m_PosScore_map[...]->head_pos are the same for a connected composant chain.
    // we don't need to check if it's the first head of all


    // we start at the second neighbor to merge all them into the first one
    // we know that we have at least 2 neighbors in the vector

    const uint8_t SIZE = neighbors.size();
    for (int i = 1; i < SIZE; i++)
    {
        m_PosScore_map[_MAIN_HEAD_POS]->score += m_PosScore_map[neighbors[i]]->score;
        m_PosScore_map[neighbors[i]]->head_pos = _MAIN_HEAD_POS;
    }

    m_PosScore_map[_MAIN_HEAD_POS]->score++;
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
    uint8_t score = 0;
    uint8_t cell_row, cell_col;

    // get the cell in which the player played
    GetPosCell(pos, cell_row, cell_col);


    // look if the player own the cell

    score += OwnCell(cell_row, cell_col) ? 9 : 0;
}


bool Player::OwnCell(uint8_t cell_row, uint8_t cell_col) const
{
    // store how many players own a slot in the cell
    std::unordered_map<uint16_t, uint16_t> slots_count;

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




    // we count which player have the most slot in the cell
    uint16_t bestOwn = 0;
    uint16_t bestID = 0;
    for (const auto& [id, count] : slots_count)
    {  
        if (count == bestOwn)
            bestID = 0;
        else if (count > bestOwn)
        {
            bestID = id;
            bestOwn = count;
        }
    }

    // the number that a player need to have to own a slot
    const float MAJORITY = 9.0f / m_NbPlayers;

    // if a player have more than the majority of the slot (5) he owns the cell
    if (bestOwn > MAJORITY)
    {
        // we return true if it's this player
        return bestID == m_ID;
    }


    // if we find an unplayed slot( == 0) we return false since there is place to play by someone.
    // there is -1 player to represent forbidden tiles
    if (slots_count.contains(0))
    {
        return false;
    }


    // if there is more than 3 players, we can own a slot even without the majority
    
    //check 
    
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
