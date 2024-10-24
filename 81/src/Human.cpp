#include "Human.h"


Human::Human(std::shared_ptr<Board> board)
    : Player {board}
{

}


void Human::Play()
{
    std::vector<int>& board = m_Board->GetMap();
    uint8_t pos = -1;
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
        {
            std::string formated_pos = "..";
            convertPosNumber_to_Char(pos, &formated_pos);
            std::cout << "\nImpossible de jouer a " << formated_pos << std::endl;
        }

        if (m_Board->IsFinish())
            correct = true;

    } while (!correct);


    /*if (m_Board->IsFinish())
        return;*/

    m_LastPos = pos;
    m_PlayedTiles.emplace_back(pos);
    m_Board->Update(pos, m_ID);

    // update datas
    Update_PositionScore_map(pos);
    Update_AreaScore(pos);
}



uint8_t Human::InputInNumber(std::string& input) const
{
    uint8_t pos = -1;

    // check that the first character is between A and I
    //      • transform this character in a number between 0 and 8

    // check that the second character is between 1 and 9
    //      • the ascii number for '0' is 48
    //      • lower the number by one so the character will be in [0,8]



    std::cout << "[player " << m_ID << "] : plays at : ";
    std::cin >> input;

    while (input.size() != 2
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