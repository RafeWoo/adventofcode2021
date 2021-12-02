// day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <filesystem>
#include <fstream>
#include <ranges>
#include <algorithm>
#include <numeric>

std::vector<int> read_input_vector(std::filesystem::path const& path)
{
    std::vector<int> result{};

    if (std::filesystem::exists(path))
    {
        auto stream = std::ifstream(path);
        int line_value = 0;
        while (stream >> line_value)
        {
            result.push_back(line_value);
        }
 
    }

    return result;
}

int main()
{

    //read in input file

    auto depths = read_input_vector("../input_files/day1.txt");

    std::adjacent_difference(std::begin(depths), std::end(depths), std::begin(depths));
    auto increase_count = std::ranges::count_if(depths, [](auto i) {return i > 0; }) -1;
    std::cout << "The number of increases is " << increase_count << "\n";


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
