#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <array>
#include <format>

import input_lib;

std::vector<int> read_crabs()
{
	
	std::ifstream file("../input_files/day7.txt");
	std::string crabs_string;

	std::getline(file, crabs_string);

	std::vector<int> crabs;
	std::string crab_string;
	std::istringstream crab_stream(crabs_string);
	while (std::getline(crab_stream, crab_string, ','))
	{
		crabs.push_back(std::stoi(crab_string));
	}

	return crabs;
}

auto calc_cost_part1(int position, std::array<int, 2000>const& crab_counts)
{

	uint64_t cost = 0;
	
		for (int pos_index = 0; pos_index < crab_counts.size(); ++pos_index)
		{
			cost += abs(pos_index - position) * crab_counts[pos_index];
		}
		

	return cost;
}

int main(void)
{
	auto crabs = read_crabs();

	auto max = std::max_element(crabs.begin(), crabs.end());

	//count how many crabs at each position
	std::array<int, 2000> position_counts{};

	for (int crab_pos : crabs)
	{
		position_counts[crab_pos]++;
	}

	//work out the cost of moving crabs to that position
	std::array<uint64_t, 2000> position_costs{};
	for (int i = 0; i < 2000; ++i)
	{
		position_costs[i] = calc_cost_part1(i, position_counts);
	}


	//find the minimum cost position
	auto min_iter = std::min_element(position_costs.begin(), position_costs.end());
	auto min_pos = min_iter - position_costs.begin();

	std::cout << std::format("The min pos is {} with fuel cost of {}\n", min_pos, *min_iter);
	

	return 0;
}