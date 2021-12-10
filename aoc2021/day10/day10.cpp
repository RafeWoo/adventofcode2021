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

uint64_t incomplete_char_score(char c)
{
	uint64_t score = 0;
	switch (c)
	{
	case '(':
		score = 1;
		break;

	case '[':
		score = 2;
		break;

	case '{':
		score = 3;
		break;

	case '<':
		score = 4;
		break;

	default:
		break;
	}

	return score;
}

uint64_t incomplete_score(std::vector<char> const& v)
{
	return std::accumulate(v.rbegin(), v.rend(), 0ULL, [](uint64_t l, char r)
		{
			return l * 5 + incomplete_char_score(r);
		}
	);
}

uint64_t calc_line_incomplete_score(std::string const& s)
{
	std::vector<char> brack_stack;

	for (auto c : s)
	{
		switch (c)
		{
		case '(':
		case '[':
		case '{':
		case '<':
			brack_stack.push_back(c);
			break;

		case ')':
		case ']':
		case '}':
		case '>':
			if (brack_stack.empty())
			{
				return 0;  
			}
			else if (!brackets_match(brack_stack.back(), c))
			{
				return 0; // illegal_char_score(c);
			}
			brack_stack.pop_back();
			break;
		}
	}

	return incomplete_score(brack_stack);
}

uint64_t calc_line_error_score(std::string const& s)
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


#if 1
	//part 2
	auto scores_view = lines 
		| std::views::filter([](const auto& s) {return calc_line_error_score(s) == 0; })
		| std::views::transform(calc_line_incomplete_score);
	auto scores = std::vector<uint64_t>{ scores_view.begin(), scores_view.end() };
	std::ranges::sort(scores);

	auto middle_score_index = scores.size() / 2; //assume odd count
	auto middle_score = scores[middle_score_index];
	std::cout << std::format("The middle score is {}\n", middle_score);
#else 

	//part 1
	auto scores = lines | std::views::transform(calc_line_error_score);
	auto error_score= std::accumulate(scores.begin(), scores.end(), 0ULL);
	
	std::cout << std::format("The score is {}\n", error_score);
#endif

	return 0;
}