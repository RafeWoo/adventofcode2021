// day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <format>

import Bingo;
import input_lib;

static inline void ltrim(std::string& s) 
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

std::vector<int> read_numbers(std::istream& stream)
{
    std::string numbers_string;
    std::getline(stream, numbers_string);
    std::istringstream ns(numbers_string);

    std::string number_string;
    std::vector<int> numbers{};
    while (std::getline(ns, number_string, ','))
    {
        numbers.push_back(std::stoi(number_string));
    }

    return numbers;
}

std::vector<std::string> tokenize( const std::string str, const std::regex re)
{
    std::sregex_token_iterator it{ str.begin(),
                             str.end(), re, -1 };
    std::vector<std::string> tokenized{ it, {} };

    // Additional check to remove empty strings
    tokenized.erase(
        std::remove_if(tokenized.begin(),
            tokenized.end(),
            [](std::string const& s) {
                return s.size() == 0;
            }),
        tokenized.end());

    return tokenized;
}

std::vector<Bingo::Board> read_boards(std::istream& stream)
{
    
    std::vector<Bingo::Board> boards;
    std::string line_string;
    while (std::getline(stream, line_string))
    {
        std::vector<int> board;
        for (int i = 0; i < 5; ++i)
        {
            std::getline(stream, line_string);


            const std::regex re(R"([\s]+)");
            auto tokens = tokenize(line_string, re);
            for (auto const tok : tokens)
            {
                board.push_back(std::stoi(tok));
            }          
        }

        boards.emplace_back( board );
    }

    return boards;
}


int play_to_win(std::vector<int> const& numbers, std::vector<Bingo::Board>& boards)
{
    for (int number : numbers)
    {
        for (auto& board : boards)
        {
            board.apply_number(number);
            if (board.is_winner())
            {
                auto score = board.calc_score();
                return score * number;
            }
        }
    }

    return 0;
}

int play_to_lose(std::vector<int> const& numbers, std::vector<Bingo::Board>&boards)
    {
    int last_winning_score = 0;
        for (int number : numbers)
        {

            auto it = std::begin(boards);
            while( it != std::end(boards) )
            {
                auto& board = *it;
            
                board.apply_number(number);
                if (board.is_winner())
                {
                    last_winning_score = board.calc_score() * number;

                    it = boards.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        return last_winning_score;
    }


int main()
{
    std::ifstream stream("../input_files/day4.txt");

    auto numbers = read_numbers(stream);
    auto boards = read_boards(stream);


#if 1
    //part 2

    auto score = play_to_lose(numbers, boards);
#else

    auto score = play_to_win(numbers, boards);
#endif


    std::cout << std::format("The scored is {} \n", score);
}
