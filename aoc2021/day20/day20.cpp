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
#include <unordered_map>
#include <iterator>
#include <bitset>
#include <functional>
#include <variant>



import input_lib;

std::array<bool, 512> parse_algorithm(std::string const& line)
{
	std::array<bool, 512> result {false};

	std::ranges::transform(line, result.begin(), [](auto c)->bool
		{
			return c == '#';
		}
	);
	
	
	return result;
}

class Grid
{
public:
	Grid(std::string const& algo, std::vector<std::string> const& inputs, size_t border)
		: m_num_rows{ inputs.size() + 2 * border }
		, m_num_cols{ inputs.front().size() + 2 * border }
		, m_grid1{ std::make_unique<uint8_t[]>(m_num_rows * m_num_cols) }
		, m_grid2{ std::make_unique<uint8_t[]>(m_num_rows * m_num_cols) }
		, m_grid{ m_grid1.get() }
		, m_grid_dst{ m_grid2.get() }
		, m_algorithm{ parse_algorithm(algo) }
	{
		
		std::fill(m_grid, m_grid + m_num_rows * m_num_cols, static_cast<uint8_t>(0));
		std::fill(m_grid2.get(), m_grid2.get() + m_num_rows * m_num_cols, static_cast<uint8_t>(0));

		size_t row = border;
		for (auto const& line : inputs)
		{
			std::ranges::transform(line, m_grid + m_num_cols * row + border, [](auto c)
				{
					return c == '#' ? 1 : 0;
				});

			++row;
		}

		
	}

	size_t pos(int x, int y) const
	{
		return y * m_num_cols + x;
	}

	size_t calc_algo_index(uint8_t* grid, int x, int y) const
	{
		//look at 9 points around x y - build up a binary number
		std::bitset<9> bits;
		bits[8] = grid[pos(x - 1, y - 1)];
		bits[7] = grid[pos(x, y - 1)];
		bits[6] = grid[pos(x + 1, y - 1)];
		bits[5] = grid[pos(x - 1, y)];
		bits[4] = grid[pos(x, y)];
		bits[3] = grid[pos(x+1, y)];
		bits[2] = grid[pos(x - 1, y + 1)];
		bits[1] = grid[pos(x , y + 1)];
		bits[0] = grid[pos(x + 1, y + 1)];

		return bits.to_ulong();
	}

	void print_grid() const
	{
		auto ptr = m_grid;

		for (int y = 0; y < m_num_rows; ++y)
		{
			for( int x =0; x < m_num_cols; ++x)
			{ 			
				auto val = ptr[y * m_num_cols + x] == 1 ? '#' : '.';
				std::cout << val;
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	void enhance()
	{
		for (int y = 1; y < m_num_rows - 1; ++y)
		{
			for (int x = 1; x < m_num_cols - 1; ++x)
			{
				m_grid_dst[pos(x, y)] = m_algorithm[calc_algo_index(m_grid, x, y)];
			}
		}

		if(m_algorithm.front() == 1 and (m_enhance_count % 2) == 0) //then turn edges on
		{ 
			for (int x = 0; x < m_num_cols; ++x)
			{
				m_grid_dst[pos(x, 0)] = 1;
				m_grid_dst[pos(x, m_num_rows - 1)] = 1;
			}

			for (int y = 0; y < m_num_rows; ++y)
			{
				m_grid_dst[pos(0, y)] = 1;
				m_grid_dst[pos(m_num_cols-1, y)] = 1;
			}
		}

		if( m_algorithm.back() == 0 and (m_enhance_count % 2) == 1) //then turn edges off
		{
			for (int x = 0; x < m_num_cols; ++x)
			{
				m_grid_dst[pos(x, 0)] = 0;
				m_grid_dst[pos(x, m_num_rows - 1)] = 0;
			}

			for (int y = 0; y < m_num_rows; ++y)
			{
				m_grid_dst[pos(0, y)] = 0;
				m_grid_dst[pos(m_num_cols - 1, y)] = 0;
			}
		}


		//swap bufffers
		std::swap(m_grid, m_grid_dst);
		++m_enhance_count;
	}

	size_t light_count() const
	{
		return std::count(m_grid, m_grid + m_num_rows * m_num_cols, 1);
	}

private:
	size_t m_num_rows;
	size_t m_num_cols;


	std::unique_ptr<uint8_t[]> m_grid1 = nullptr;
	std::unique_ptr<uint8_t[]> m_grid2 = nullptr;
	uint8_t* m_grid = nullptr;
	uint8_t* m_grid_dst = nullptr;

	std::array<bool, 512> m_algorithm;
	int m_enhance_count = 0;
};

int main(void)
{

	auto input_stream = std::ifstream("../input_files/day20.txt");

	std::string first_line;
	std::getline(input_stream, first_line);
	auto the_algorithm = parse_algorithm(first_line);

	std::string dummy;
	std::getline(input_stream, dummy);

	//read remaining lines as starting grid
	auto lines = input::read_vector(input_stream, [](auto const& s) {return s; });
	auto grid = Grid(first_line, lines,54);

	for( int i = 0; i<50;++i)
	{ 
		grid.enhance();
	}
	//grid.print_grid();
	
	//grid.print_grid();
	//grid.enhance();
	//grid.print_grid();
	//auto bob =grid.calc_algo_index(3,3);
	auto light_count = grid.light_count();
	std::cout << std::format("The number of lights is {}", light_count);
	return 0;
} 