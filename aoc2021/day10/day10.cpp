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
#include <filesystem>

import input_lib;

uint64_t illegal_char_score(char c)
{
	uint64_t score = 0;
	switch (c)
	{
	case ')':
		score = 3;
		break;

	case ']':
		score = 57;
		break;

	case '}':
		score = 1197;
		break;

	case '>':
		score = 25137;
		break;

	default:
		break;
	}

	return score;
}

bool brackets_match(char open, char close)
{
	switch (close)
	{
	case ')':
		return open == '(';


	case ']':
		return open == '[';


	case '}':
		return open == '{';


	case '>':
		return open == '<';
	default:
		return false;
		
	}
}

uint64_t calc_line_score(std::string const& s)
{

	std::stack<char> brack_stack;

	for (auto c : s)
	{
		switch (c)
		{
		case '(':
		case '[':
		case '{':
		case '<':
			brack_stack.push(c);
			break;

		case ')':					
		case ']':			
		case '}':			
		case '>':
			if (brack_stack.empty())
			{
				return 0;
			}
			else if( !brackets_match(brack_stack.top(), c ) )
			{ 
				return illegal_char_score(c);
			}
			brack_stack.pop();
			break;
		}
	}

	return 0;
}

std::string copy_line(std::string const& s) 
{
	return s; 
}

int main(void)
{
	auto lines = input::read_vector( "../input_files/day10.txt", copy_line);

	auto scores = lines | std::views::transform(calc_line_score);
	auto error_score= std::accumulate(scores.begin(), scores.end(), 0ULL);
	
	std::cout << std::format("The score is {}\n", error_score);
	

	return 0;
}