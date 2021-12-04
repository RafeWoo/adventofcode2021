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

#if 1
    //part2
    int64_t aim = 0;
    int64_t position = 0;
    int64_t depth = 0;

    std::ranges::for_each(readings, [&aim, &position, &depth](auto const& reading)
        {
            auto X = reading.second;

            switch (reading.first)
            {
            case Direction::down:
                aim += X;
                break;

            case Direction::up:
                aim -= X;
                break;

            case Direction::forward:
                position += X;
                depth += aim * X;
                break;
            }
        });
    auto answer = position * depth;
    std::cout << "Hello World! result = " << answer << "\n";
#else
    //part1

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
#endif
}

