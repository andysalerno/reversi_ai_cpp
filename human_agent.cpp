#include "human_agent.hpp"
#include <iostream>
#include <string>

coord human_agent::pick_move(board &_board, std::vector<coord> &legal_moves)
{
    std::string user_input;
    while (true)
    {
        std::cout << "Enter a move x,y: ";
        std::cin >> user_input;

        if (user_input.length() != 3 || user_input.at(1) != ',' || !isdigit(user_input.at(0)) || !isdigit(user_input.at(2)))
        {
            std::cout << "Invalid input formatting.  Use format 'x,y'.\n";
            continue;
        }

        int user_x = user_input.at(0) - '0';
        int user_y = user_input.at(2) - '0';

        if (!_board.is_in_bounds({user_x, user_y}))
        {
            std::cout << user_x << ", " << user_y << " is not in board bounds.\n";
            continue;
        }

        return {user_x, user_y};
    }
}