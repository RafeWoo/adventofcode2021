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


template<typename TPred>
auto calc_rating(std::vector<report_bits> const& report, TPred calc_filter)
{
    int i = 0;
    auto bits_range = report;
    while (i < BIT_WIDTH && bits_range.size() > 1)
    {
        int count = 0;
        std::ranges::for_each(bits_range, [&count, i](const auto& bits)
            {
                count += bits[BIT_WIDTH - 1 - i] ? 1 : -1;
            });

        auto filter_bit = calc_filter(count);// >= 0;

        bits_range.erase(
            std::remove_if(bits_range.begin(), bits_range.end(), [i, filter_bit](const auto& bits)
                { return bits[BIT_WIDTH - 1 - i] != filter_bit; })

            , bits_range.end());


        ++i;
    }

    auto fred = bits_range.size();
    auto rating = bits_range.front().to_ullong();

    return rating;
}

int main()
{
    //auto report = input::read_vector("../input_files/day3_example.txt", string_to_bits);
    auto report = input::read_vector("../input_files/day3.txt", string_to_bits);

#if 1

    //max bitwidth iterations

    auto O2_rating = calc_rating(report, [](auto const& count) {return count >= 0; });
    auto CO2_rating = calc_rating(report, [](auto const& count) {return count < 0; });

    //in each iteration i
      //count most bits 
      //filter out those that dont have filter bit
    auto answer = O2_rating * CO2_rating;
    std::cout << std::format("The answer is {}\n", answer);

#else
    //part 1

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
#endif
}


