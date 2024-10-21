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
    m_LastPos = pos;
    m_PlayedTiles.emplace_back(pos);
    // return if there is an error (in future)
    // TODO : handle potential errors
    m_Board->Update(pos, m_ID);
    return true;
}

float Player::score()
{
    return AreaScore() + LinearScore();
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
        pos = InputInNumber(input);

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
    std::cout << "[player " << m_ID << "] : plays at : ";
    std::cin >> input;

    try
    {
        pos = std::stoi(input);
    }
    catch (std::invalid_argument const& ex)
    {
        std::cerr << "invalid argument " << ex.what() << std::endl;
        pos = -1;
    }
    catch (std::out_of_range const& ex)
    {
        std::cerr << "out of range " << ex.what() << std::endl;
        pos = -1;
    }

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
    
    const uint8_t SIZE = neighbors.size();

    switch (SIZE) {

    // if there is 1 neighbor, we add the current position to the chain and we update the score
    case 1:
        m_PosScore_map[neighbors[0]]->score += 1;
        m_PosScore_map[pos] = m_PosScore_map[neighbors[0]];
        std::cout << "1 neighbor : Adding to the branch : " << (int)m_PosScore_map[neighbors[0]]->head_pos << std::endl;
        break;

    // if there is 0, we create a new chain with a score of 1
    case 0:
        m_PosScore_map[pos] = std::make_shared<Score_Head>(pos, 1);
        std::cout << "0 neighbor : Creating a new branch branch : " << (int)m_PosScore_map[pos]->head_pos << std::endl;
        break;

    // if there is more than 2, we merge chains
    default:
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
    uint8_t score = 0;
    // size is between 2 and 4
    const uint8_t SIZE = neighbors.size();


    // the head's position of the first chain
    const uint8_t HEAD_POS = m_PosScore_map[neighbors[0]]->head_pos;


    

    // compute the total score
    for (const auto& neighbor : neighbors)
    {
        score += m_PosScore_map[neighbor]->score;
    }
    // adding one for the newly placed node
    score++;


    // updating datas in the Pos/Head map
    m_PosScore_map[neighbors[0]]->score = score;
    for (int i = 1; i < SIZE; i++)
    {
        m_PosScore_map[neighbors[i]]->head_pos = HEAD_POS;
        m_PosScore_map[neighbors[i]]->score = score;
    }
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













float Player::AreaScore() const
{
    std::vector<int>& board = m_Board->GetMap();
    float score = 0;
    std::array<std::array<int, 9>, 9> cells;


    // slice the board into nine 3x3 arrays
    sliceBoard(board, &cells);


    // look though each cells and return the number of arrays that this player owns.
    for (int i = 0; i < 9; i++)
    {
        score += OwnCell(cells[i]) ? 4.5f : 0;
    }

    //return the score
    return score;
}


void Player::sliceBoard(const std::vector<int>& board, std::array<std::array<int, 9>, 9>* const cells) const
{

    /*
    *   posibilité de faire un boucle qui s'incrémente 3 par 3. Avec 3 affectations par tour de boucle 
    */



    const int size = board.size();
    const uint8_t SQRT_SIZE= sqrt(size);

    // the row and the column of the position in the board
    int row{};
    int column{};
         
    // the row and the column in the cell of the position in the board
    int relative_row{};
    int relative_column{};
         
    // the position in the cell based on the relative row and column
    int relative_pos{};
         
    // the id of the the cell in which the position is.
    int cell_num{};


    for (int i = 0; i < size; i++)
    {
        // convert each position in 2 dimensional board
        row = i / SQRT_SIZE;
        column = (i - row * SQRT_SIZE);

        // convert the row and the column relative to a cell
        relative_row = row % 3;
        relative_column = column % 3;

        // convert the relative row and column into a single position in a flatten-cell
        relative_pos = relative_row * 3 + relative_column;
        

        // we make all indices/positions to point to the first slot in their cell.
        cell_num = i - ((relative_pos * 3) - 2 * (relative_pos % 3));
        /*  0    3   6
         *  27  30  33
         *  54  57  60
        */      
                
        cell_num /= 3;
        /*  0    1    3
         *   9   10   11
         *  18   19   20
        */      

        cell_num -= (i / 27) * 6;
        /*    0   1   2          
          *   3   4   5          
          *   6   7   8          
         */                      

  
        (*cells)[cell_num][relative_pos] = board[i];
    }
}

bool Player::OwnCell(const std::array<int, 9>& cell) const
{
    // store how many players own a slot in the cell
    std::unordered_map<uint16_t, uint16_t> slots_count;

    // look at the cell to fill the count map
    for (int i = 0; i < 9; i++)
    {
        // add 1 to the count, and checking if the key doesn't exist before
        slots_count[cell[i]] = slots_count.contains(cell[i]) ? slots_count.at(cell[i]) + 1 :  1;
    }

    // if we find an unplayed slot( == 0) we return false since there is place to play by someone.
    if (slots_count.contains(0))
    {
        return false;
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

    // we return true if it's this player
    return bestID == m_ID;
}