// day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
import input_lib;

#include <iostream>
#include <sstream>
#include "magic_enums.hpp"

#include <ranges>
#include <algorithm>

enum class Direction
{
    down,
    forward,
    up,
};


std::pair<Direction,int> read_line(std::string const& s)
{
    std::stringstream ss(s);

    std::string dir_string;
    int distance;
    ss >> dir_string >> distance;

    auto direction = magic_enum::enum_cast<Direction>(dir_string);
    return std::make_pair( *direction, distance);
}


int main()
{
    auto readings = input::read_vector("../input_files/day2.txt", read_line);

    uint64_t distance = 0;
    int64_t  depth = 0;

    std::ranges::for_each(readings, [&distance, &depth](auto const& reading)
        {
            switch (reading.first)
            {
            case Direction::down:
                depth += reading.second;
                break;

            case Direction::up:
                depth -= reading.second;
                break;

            case Direction::forward:
                distance += reading.second;
                break;
            }
        });

    auto answer = distance * depth;
    std::cout << "Hello World! result = " << answer << "\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
