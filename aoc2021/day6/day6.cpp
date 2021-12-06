#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ranges>
#include <format>
#include <algorithm>

import input_lib;

std::vector<int> read_fishes()
{
	std::vector<int> fish;
	std::ifstream file("../input_files/day6.txt");
	std::string fishes_string;
	std::string fish_string;
	std::getline(file, fishes_string);
	std::istringstream fish_stream(fishes_string);
	while (std::getline(fish_stream, fish_string, ','))
	{
		fish.push_back(std::stoi(fish_string));
	}

	return fish;
}

static constexpr int NEW_FISH_VALUE = 8;
static constexpr int RESET_FISH_VALUE = 6;

std::vector<int> simulate_day(std::vector<int>&& fish_in)
{
	std::vector<int> new_fish;

	auto sim_fish = [&new_fish](int f)
	{
		if (f == 0)
		{
			new_fish.push_back(NEW_FISH_VALUE);
			return RESET_FISH_VALUE;
		}
		else
		{
			return f - 1;
		}
	};

	std::ranges::transform(fish_in, std::begin(fish_in), sim_fish);

	fish_in.insert(std::end(fish_in), std::begin(new_fish), std::end(new_fish));

	return fish_in;
}
int main(void)
{
	auto fish = read_fishes();

	int day = 0;
	for (; day < 80; ++day)
	{
		fish = simulate_day(std::move(fish));
	}
	std::cout << std::format("THere are {} fish after {} days\n", fish.size(), day);
	return 0;
}