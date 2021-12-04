// day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <vector>
#include <format>
#include <ranges>
#include <algorithm>
#include <array>

import input_lib;

static constexpr size_t BIT_WIDTH = 12;

using report_bits = std::bitset<BIT_WIDTH>;

report_bits string_to_bits(const std::string& s)
{
    return report_bits{ s };
}

int main()
{
    //auto report = input::read_vector("../input_files/day3_example.txt", string_to_bits);
    auto report = input::read_vector("../input_files/day3.txt", string_to_bits);

    std::array<int, BIT_WIDTH> counts{};

    std::ranges::for_each(report, [&counts](const auto& bits)
        {
            for (int i = 0; i < BIT_WIDTH; ++i)
            {
                counts[i] += bits[i] ? 1 : -1;
            }

        });

    report_bits gamma{};

    for (int i = 0; i < BIT_WIDTH; ++i)
    {
        gamma[i] = counts[i] > 0;
    }

    auto gdec = gamma.to_ullong();

    report_bits epsilon = gamma.flip();
    auto answer = gdec * epsilon.to_ullong();
    std::cout << std::format( "The answer is {}\n", answer);
}


