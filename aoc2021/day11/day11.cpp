#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <algorithm>
#include <format>
#include <set>
#include <map>
#include <limits>
#include <array>
#include <stack>
#include <numeric>
#include <assert.h>


static constexpr int FLASH_TRIGGER_LEVEL = 9;
class Octopus
{
public:
	Octopus(int starting_level)
		: m_energy{ starting_level }
	{}

	int energy_level() const
	{
		return m_energy;
	}

	void increase_energy()
	{
		++m_energy;
	}

	bool should_flash() const
	{
		return (m_energy > FLASH_TRIGGER_LEVEL) && !m_has_flashed;
	}

	void flash()
	{
		m_has_flashed = true;
	}

	void reset()
	{
		if (m_has_flashed)
		{
			m_energy = 0;
			m_has_flashed = false;
		}
		else
		{
			assert(m_energy <= FLASH_TRIGGER_LEVEL);
		}
	}

private:
	int m_energy{ 0 };
	bool m_has_flashed{ false };
};


static constexpr size_t GRID_SIZE = 100;
using OctoGrid = std::array<Octopus, GRID_SIZE>;

OctoGrid load_input()
{
#if 1
	return OctoGrid{
5,7,2,3,5,7,3,1,5,8,
3,1,5,4,7,4,8,5,6,3,
4,7,8,3,5,1,4,8,7,8,
3,8,4,8,1,4,2,3,7,5,
3,6,3,7,7,2,4,1,5,1,
8,5,8,3,1,7,2,4,8,4,
7,7,4,7,4,4,4,1,8,4,
1,6,1,3,3,6,7,8,8,2,
6,2,2,8,6,1,4,2,2,7,
4,7,3,2,2,2,5,3,3,4,
	};
#else
	return OctoGrid{
		5,4,8,3,1,4,3,2,2,3,
2,7,4,5,8,5,4,7,1,1,
5,2,6,4,5,5,6,1,7,3,
6,1,4,1,3,3,6,1,4,6,
6,3,5,7,3,8,5,4,7,8,
4,1,6,7,5,2,4,6,4,5,
2,1,7,6,8,4,1,7,2,1,
6,8,8,2,8,8,1,1,3,4,
4,8,4,6,8,4,8,5,5,4,
5,2,8,3,7,5,1,5,2,6,
	};
#endif
}


std::vector<int> neighbours(int pos)
{
	std::vector<int> adjacent{};

	static constexpr int UL = -11;
	static constexpr int U = -10;
	static constexpr int UR = -9;
	static constexpr int L = -1;
	static constexpr int R = 1;
	static constexpr int DL = 9;
	static constexpr int D = 10;
	static constexpr int DR = 11;

	int row = pos / 10;
	int col = pos % 10;

	if (row > 0)
	{
		if (col > 0)
		{
			adjacent.push_back(pos + UL);
		}
		adjacent.push_back(pos + U);
		if (col < 9)
		{
			adjacent.push_back(pos + UR);
		}
	}
	if (col > 0)
	{
		adjacent.push_back(pos + L);
	}
	if (col < 9)
	{
		adjacent.push_back(pos + R);
	}
	if (row < 9)
	{
		if (col > 0)
		{
			adjacent.push_back(pos + DL);
		}
		adjacent.push_back(pos + D);

		if (col < 9)
		{
			adjacent.push_back(pos + DR);
		}
	}

	return adjacent;
}

using VisitedMap = std::array<bool, GRID_SIZE>;
int64_t flash_grid(OctoGrid& grid)
{
	int64_t flash_count = 0;

	VisitedMap visited{};

	std::stack<int> flash_stack;

	//get original flashers
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		if (grid[i].should_flash())
		{
			flash_stack.push(i);
			visited[i] = true;
		}
	}

	while (!flash_stack.empty())
	{
		int flasher = flash_stack.top();
		flash_stack.pop();

		grid[flasher].flash();
		flash_count++;
		auto adj = neighbours(flasher);

		for (auto n : adj)
		{
			grid[n].increase_energy();
			if (grid[n].should_flash() && !visited[n])
			{
				flash_stack.push(n);
				visited[n] = true;
			}
		}
	}

	return flash_count;
}

void print_grid(OctoGrid const& grid)
{
	for (int i = 0; i < 100; ++i)
	{
		char digit = (grid[i].energy_level() < 10) ? '0' + grid[i].energy_level() : '*';
		std::cout <<  digit;
		if (i % 10 == 9)
		{
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}

int main(void)
{
		auto grid = load_input();

		int64_t flash_count = 0;

		//print_grid(grid);
		for (int i = 0; i < 100; ++i)
		{
			//each iter
			std::ranges::for_each(grid, [](auto& oct) { oct.increase_energy(); });
			//print_grid(grid);
			flash_count += flash_grid(grid);
			//print_grid(grid);
			std::ranges::for_each(grid, [](auto& oct) { oct.reset(); });
			//print_grid(grid);

			int ten = 9;
			++ten;

		}

		std::cout << std::format("The flash count is {}", flash_count);

		return 0;
}


