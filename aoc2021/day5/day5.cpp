// day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <format>

import input_lib;

struct Coord
{
    int x;
    int y;
};

std::pair<Coord, Coord> read_line(std::string const& s)
{
    std::regex word_regex{ R"~(^([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+))~" };

    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;
    std::smatch sm;
    if (std::regex_match(s, sm, word_regex))
    {
        x0 = std::stoi(sm[1]);
        y0 = std::stoi(sm[2]);

        x1 = std::stoi(sm[3]);
        y1 = std::stoi(sm[4]);
    }

return { {x0,y0}, {x1,y1} };
}

static constexpr size_t GRID_WIDTH = 1000;
static constexpr size_t GRID_HEIGHT = 1000;

void print_answer1(std::vector<std::pair<Coord, Coord>> lines)
{
    auto p_grid = std::make_unique < std::array<uint8_t, GRID_WIDTH* GRID_HEIGHT> >();
    auto& grid = *p_grid;

    for (auto const& line : lines)
    {
        if (line.first.x == line.second.x)
        {
            //vertical line
            auto top = std::min(line.first.y, line.second.y);
            auto bot = std::max(line.first.y, line.second.y);
            int x = line.first.x;
            for (int y = top; y <= bot; ++y)
            {
                grid[y * GRID_WIDTH + x]++;
            }
        }
        else if (line.first.y == line.second.y)
        {
            //horizontal line
            int y = line.first.y;
            auto left = std::min(line.first.x, line.second.x);
            auto right = std::max(line.first.x, line.second.x);

            for (int x = left; x <= right; ++x)
            {
                grid[y * GRID_WIDTH + x]++;
            }
        }
        else
        {
            //ignore
        }
    }

    auto answer = std::count_if(std::begin(grid), std::end(grid), [](const auto& val) {return val >= 2; });
    std::cout << std::format("The answer is {}\n", answer);
}

void print_grid(std::array<int, GRID_WIDTH* GRID_HEIGHT> const& grid)
{
    for (int row = 0; row < GRID_HEIGHT; ++row)
    {
        for (int col = 0; col < GRID_WIDTH; ++col)
        {
            std::cout << grid[row * GRID_WIDTH + col] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void print_answer2(std::vector<std::pair<Coord, Coord>> lines)
{
    auto p_grid = std::make_unique < std::array<int, GRID_WIDTH* GRID_HEIGHT> >();
    auto& grid = *p_grid;

    for (auto const& line : lines)
    {
        if (line.first.x == line.second.x)
        {
            //vertical line
            auto top = std::min(line.first.y, line.second.y);
            auto bot = std::max(line.first.y, line.second.y);
            int x = line.first.x;
            for (int y = top; y <= bot; ++y)
            {
                grid[y * GRID_WIDTH + x]++;
            }
        }
        else if (line.first.y == line.second.y)
        {
            //horizontal line
            int y = line.first.y;
            auto left = std::min(line.first.x, line.second.x);
            auto right = std::max(line.first.x, line.second.x);

            for (int x = left; x <= right; ++x)
            {
                grid[y * GRID_WIDTH + x]++;
            }
        }
        else
        {
            //assume diagonal
            if (line.first.x < line.second.x) //going right
            {
                auto left = line.first.x;
                auto right = line.second.x;

                if (line.first.y < line.second.y)  //going down
                {
                    auto top = line.first.y;
                    auto bot = line.second.y;

                    for (int x = left, y = top; y <= bot && x <= right; ++y, ++x)
                    {
                        grid[y * GRID_WIDTH + x]++;
                    }
                }
                else   //going up
                {
                    auto top = line.second.y;
                    auto bot = line.first.y;  

                    for (int x = left, y = bot; y >= top && x <= right; --y, ++x)
                    {
                        grid[y * GRID_WIDTH + x]++;
                    }
                }
            }
            else  //going left
            {
                auto left = line.second.x;
                auto right = line.first.x;

                if (line.first.y < line.second.y)  //going down
                {
                    auto top = line.first.y;
                    auto bot = line.second.y;
 
                    for (int x = left, y = bot; y >= top && x <= right; --y, ++x)
                    {
                        grid[y * GRID_WIDTH + x]++;
                    }
                }
                else   //going up
                {
                    auto top = line.second.y; 
                    auto bot = line.first.y;
                  
                    for (int x = left, y = top; y <= bot && x <= right; ++y, ++x)
                    {
                        grid[y * GRID_WIDTH + x]++;
                    }
                }
            }
                     
        }     
    }
    
    auto answer = std::count_if(std::begin(grid), std::end(grid), [](const auto& val) {return val >= 2; });
    std::cout << std::format("The answer is {}\n", answer);
}


int main()
{
    //using namespace std::literals::string_literals;
    
    auto coords = input::read_vector("../input_files/day5.txt", read_line);

    //print_answer1(coords);
    print_answer2(coords);
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
