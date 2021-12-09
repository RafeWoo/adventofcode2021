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

import input_lib;


template<size_t COLS, size_t ROWS>
class HeightMap
{
public:
	using Point = std::pair<int, int>;

	HeightMap(std::vector<std::string> const& lines)
	{
		std::fill(m_values.begin(), m_values.end(), std::numeric_limits<int>::max());
		
		//for each line
		for (int y_pos = 0; y_pos < lines.size(); ++y_pos)
		{
			auto line = lines[y_pos];
			for (int x_pos = 0; x_pos < line.size(); ++x_pos)
			{
				value_at(x_pos, y_pos) = line[x_pos] - '0';
			}
		}
	}

	bool is_low_point(int x, int y) const
	{
		auto val = value_at(x, y);

		return (val < value_at(x + 1, y))
			and (val < value_at(x, y + 1))
			and (val < value_at(x - 1, y))
			and (val < value_at(x, y - 1));
	}

	size_t low_points_count() const
	{
		size_t low_points = 0;

		for (int y_pos = 0; y_pos < ROWS; ++y_pos)
		{
			for (int x_pos = 0; x_pos < COLS; ++x_pos)
			{
				if (is_low_point(x_pos, y_pos))
				{
					++low_points;
				}
			}
		}

		return low_points;
	}

	size_t low_points_risk() const
	{
		size_t risk = 0;

		for (int y_pos = 0; y_pos < ROWS; ++y_pos)
		{
			for (int x_pos = 0; x_pos < COLS; ++x_pos)
			{
				if (is_low_point(x_pos, y_pos))
				{
					risk += value_at(x_pos,y_pos) + 1;
				}
			}
		}

		return risk;
	}

	std::vector< Point > low_points() const
	{
		std::vector<Point> lows{};

		for (int y_pos = 0; y_pos < ROWS; ++y_pos)
		{
			for (int x_pos = 0; x_pos < COLS; ++x_pos)
			{
				if (is_low_point(x_pos, y_pos))
				{
					lows.emplace_back(x_pos, y_pos);
				}
			}
		}
		return lows;
	}

	std::set< Point> calc_basin(Point lp)
	{
		std::stack<Point> unexplored;
		std::set<Point> visited;
		std::set<Point> basin;

		unexplored.push(lp);

		while (!unexplored.empty())
		{
			auto p = unexplored.top();
			unexplored.pop();

			basin.insert(p);

			//for each neighbour
			for (auto neighbour : neighbours(p))
			{
				if (value_at(neighbour) < 9 && !visited.contains(neighbour))
				{				
					unexplored.push(neighbour);
				}
				visited.insert(neighbour);
			}
		}

		return basin;
	}

private:
	std::vector<Point> neighbours(Point p) const
	{
		std::vector<Point> ns;

		ns.emplace_back( p.first + 1, p.second);
		ns.emplace_back( p.first , p.second + 1);
		ns.emplace_back( p.first - 1, p.second);
		ns.emplace_back( p.first , p.second - 1);

		return ns;
	}
	int value_at(Point p) const
	{
		return value_at(p.first, p.second);
	}
	int value_at(int x, int y) const
	{
		return m_values[(y + 1) * (COLS + 2) + (x + 1)];
	}

	int& value_at(int x, int y)
	{
		return m_values[(y + 1) * (COLS + 2) + (x + 1)];
	}
	std::array<int, (ROWS + 2)* (COLS + 2)> m_values;
};

int main(void)
{
	std::ifstream file("../input_files/day9.txt");
	auto lines = input::read_vector(file, [](const auto& s) {return s; });

	auto map = HeightMap<100,100>{ lines };

	//auto risk_level = map.low_points_risk();
	//std::cout << std::format("The number of low _points is {}\n", risk_level);

	auto basin_sizes = std::vector<size_t>{};
	auto lows = map.low_points();
	for (auto const& point : lows)
	{
		basin_sizes.push_back(map.calc_basin(point).size() );
	}
	std::ranges::sort(basin_sizes, std::greater());

	auto three_largest_product = basin_sizes[0] * basin_sizes[1] * basin_sizes[2];

	std::cout << std::format("The size of 3 largest basins is {}\n", three_largest_product);
	return 0;
}