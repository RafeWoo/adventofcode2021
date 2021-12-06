#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ranges>
#include <format>
#include <algorithm>
#include <array>
#include <numeric>

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


class FishTank
{
public:
	FishTank(std::vector<int> const& fish)
	{
		for (int fish_age : fish)
		{
			m_fish_counts[fish_age]++;
		}
	}

	void simulate_day()
	{
		auto repo_count = m_fish_counts[0];
		m_fish_counts[0] = 0;
		std::rotate(m_fish_counts.begin(), m_fish_counts.begin()+1, m_fish_counts.end());
		m_fish_counts[RESET_FISH_VALUE] += repo_count;
		m_fish_counts[NEW_FISH_VALUE] += repo_count;
	}

	auto total_fish_count() const
	{
		return std::accumulate(m_fish_counts.begin(), m_fish_counts.end(), 0ULL);
	}
private:
	static constexpr size_t AGE_RANGE = 9;
	std::array<uint64_t, AGE_RANGE> m_fish_counts{};
};

int main(void)
{
	auto fish = read_fishes();

#if 1
	auto tank = FishTank{ fish };
	for (int day = 0; day < 256; ++day)
	{
		tank.simulate_day();
	}
	auto result = tank.total_fish_count();
	std::cout << std::format("THere are {} fish after 256 days\n", result);
#else
	int day = 0;
	for (; day < 80; ++day)
	{
		fish = simulate_day(std::move(fish));
	}
	std::cout << std::format("THere are {} fish after {} days\n", fish.size(), day);

#endif
	return 0;
}