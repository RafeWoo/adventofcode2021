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


import input_lib;

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

	std::vector < std::string > m_inputs;
	std::vector < std::string > m_outputs;
};


Lights string_to_lights( std::string s)
{	
	return Lights{s};
}

int main(void)
{
	auto file = std::ifstream("../input_files/day8.txt");
	auto lights = input::read_vector(file, string_to_lights);

	int64_t output_digit_count = 0;
	for (const auto& light : lights)
	{
		output_digit_count += std::ranges::count_if(light.m_outputs, [](const auto& s)
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

	std::cout << std::format("The output is {}", output_digit_count) << std::endl;
	return 0;
}