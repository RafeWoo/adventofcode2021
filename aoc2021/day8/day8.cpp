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


import input_lib;


#if 1
//part2
class Lights
{

public:
	Lights(const std::string& s)
	{
		std::vector < std::set<char> > inputs;
		auto* dest = &inputs;

		auto ss = std::istringstream(s);
		std::string word;
		while (std::getline(ss, word, ' '))
		{
			if (word == "|")
			{
				dest = &m_outputs;
				continue;
			}

			dest->emplace_back(std::set(word.begin(), word.end()));
		}

		calc_map(std::move(inputs));
	}


	using DisSet = std::set<char>;

	static bool set_has_size(DisSet const& s, size_t size)
	{
		return s.size() == size;
	}




	void calc_map(std::vector < DisSet >&& inputs)
	{
		DisSet one, two, three, four, five, six, seven, eight, nine, zero;

		//find 1,7,4,8
		for (auto it = inputs.begin(); it != inputs.end();)
		{
			if (set_has_size(*it, 2))
			{
				one = *it;				
				it = inputs.erase(it);
			}
			else if (set_has_size(*it, 3))
			{
				seven = *it;
				it = inputs.erase(it);
			}
			else if (set_has_size(*it, 4))
			{
				four = *it;
				it = inputs.erase(it);
			}
			else if (set_has_size(*it, 7))
			{
				eight = *it;
				it = inputs.erase(it);
			}
			else
			{
				++it;
			}

		}
		
		//find 3 //size 5 and contains one's char
		auto it3 = std::find_if(inputs.begin(), inputs.end(), [&one](const auto& s)
			{
				return s.size() == 5 and
					std::ranges::includes(s, one);
			}
		);
		three = *it3;
		inputs.erase(it3);
		
		//find set of horizontal bars
		std::vector<char> hor;
		std::ranges::set_difference(three, one, std::inserter(hor, hor.begin()));

		//find middle bar
		std::vector<char> bar;
		std::ranges::set_intersection(four,hor, std::inserter(bar, bar.begin()));


	
		//find zero // size 6 and does not contain middle bar
		auto zero_it = std::find_if(inputs.begin(), inputs.end(), [&bar](const auto& s)
			{
				return s.size() == 6 and !std::ranges::includes(s, bar);
			}
		);
		zero = *zero_it;
		inputs.erase(zero_it);

	
		//nine is size 6 and includes one
		auto nine_it = std::find_if(inputs.begin(), inputs.end(), [&one](const auto& s)
			{
				return s.size() == 6 and std::ranges::includes(s, one);
			});
		nine = *nine_it;
		inputs.erase(nine_it);


		//six is only set of size 6 left
		auto six_it = std::find_if(inputs.begin(), inputs.end(), [&one](const auto& s)
			{
				return s.size() == 6;
			});
		six = *six_it;
		inputs.erase(six_it);

		//diff of eight and six gives top right
		std::vector<char> top_right;
		std::ranges::set_difference(eight,
			six,
			std::inserter(top_right, top_right.begin()));
		
		//tow contains top right
		//five is remainder
		for (auto& dis : inputs)
		{
			if (std::ranges::includes(dis, top_right))
			{
				two = dis;
			}
			else
			{
				five = dis;
			}
		}
	


		

#if 1
		m_io_map[one] = 1;
		m_io_map[two] = 2;
		m_io_map[three] = 3;
		m_io_map[four] = 4;
		m_io_map[five] = 5;
		m_io_map[six] = 6;
		m_io_map[seven] = 7;
		m_io_map[eight] = 8;
		m_io_map[nine] = 9;
		m_io_map[zero] = 0;
#endif
	}

	int64_t output_count() const
	{
		int64_t result = 0;

		for (auto const& output : m_outputs)
		{
			result = m_io_map.find(output)->second + result * 10;
		}
		return result;
	}

private:

	std::map< std::set<char>, int > m_io_map;
	std::vector < std::set<char> > m_outputs;
};
#else
//part1
class Lights
{

public:
	Lights(const std::string& s)
	{

		auto* dest = &m_inputs;
#if 0
		auto sv = std::string_view(s);



		for (const auto token : sv | std::views::lazy_split(' ') | std::views::common)
		{
			//inputs.emplace_back(std::string{ token.begin(), token.end() } );
			//std::cout << std::string_view(token.begin(), token.end()) << std::endl;
			auto newb = std::string{};
			for (auto a : token)
			{
				newb.push_back(a);

			}


			dest->push_back(newb);

		}
#endif

		auto ss = std::istringstream(s);
		std::string word;
		while (std::getline(ss, word, ' '))
		{
			if (word == "|")
			{
				dest = &m_outputs;
				continue;
			}

			dest->emplace_back(std::move(word));
		}

	}

	int64_t output_count() const
	{
		return std::ranges::count_if(m_outputs, [](const auto& s)
			{
				switch (s.size())
				{
				case 2:
				case 3:
				case 4:
				case 7:
					return true;

				default:
					return false;

				}
			}
		);
	}

private:
	std::vector < std::string > m_inputs;
	std::vector < std::string > m_outputs;
};
#endif

Lights string_to_lights(std::string s)
{
	return Lights{ s };
}

int main(void)
{
	auto file = std::ifstream("../input_files/day8.txt");
	auto lights = input::read_vector(file, string_to_lights);

	int64_t output_digit_count = 0;
	for (const auto& light : lights)
	{
		output_digit_count += light.output_count();
	}

	std::cout << std::format("The output is {}", output_digit_count) << std::endl;
	return 0;
}