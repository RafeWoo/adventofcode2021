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

    auto original_depths = read_input_vector("../input_files/day1.txt");

    auto depths = original_depths;
    std::adjacent_difference(std::begin(depths), std::end(depths), std::begin(depths));
    auto increase_count = std::ranges::count_if(depths, [](auto i) {return i > 0; }) -1;
    std::cout << "The number of increases is " << increase_count << "\n";

    
    auto depths2 = original_depths;

    int current_sum = depths2[0] + depths2[1] + depths2[2];
    int increase_count2 = 0;
    for (int i = 1; i < depths2.size() - 2; ++i)
    {
        int next_sum = current_sum + depths2[i + 2] - depths2[i - 1];
        if (next_sum > current_sum)
        {
            ++increase_count2;
        }
        current_sum = next_sum;
    }
    std::cout << "Part2 : The number of increases is " << increase_count2 << "\n";

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
