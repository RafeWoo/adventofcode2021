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
#include <regex>

import input_lib;

enum class FoldDir
{
	Up,
	Left
};

using Fold = std::pair<FoldDir, int>;
using Dot = std::pair<int, int>;
std::vector<Dot> read_dots(std::istream& s)
{
	std::vector<Dot> dots;

	std::regex dot_pattern{"^([0-9]+),([0-9]+)$"};
	std::string line;	
	while (getline(s, line) && !line.empty())
	{
		std::smatch result;
		if (std::regex_match(line, result, dot_pattern))
		{
			dots.emplace_back( std::stoi(result[1]), std::stoi(result[2]) );
		}
	}

	return dots;
}

Fold str_to_fold(std::string const& s)
{
	Fold fold{};
	std::regex fold_pattern{ "^fold along ([xy])=([0-9]+)$" };
	
	std::smatch result;
	if (std::regex_match(s, result, fold_pattern))
	{	
		fold = {
			result[1] == "x" ? FoldDir::Left : FoldDir::Up
		 , std::stoi(result[2]) };
	}

	return fold;
}

std::vector<Fold> read_folds(std::istream& s)
{
	return input::read_vector(s, str_to_fold);
}


std::vector<Dot> apply_fold(Fold const& fold, std::vector<Dot>&& dots)
{
	switch (fold.first)
	{
	case FoldDir::Up: //x stays the same y > fold line are changed by 2(y-foldline)
		std::ranges::transform(dots, dots.begin(), [fl = fold.second](auto const& d) {return std::make_pair(d.first, d.second > fl ? d.second - 2 * (d.second - fl) : d.second); });
		break;

	case FoldDir::Left: //y stays the same x > fold line are changed by 2(x-foldline)
		std::ranges::transform(dots, dots.begin(), [fl = fold.second](auto const& d) {return std::make_pair(d.first > fl ? d.first - 2 * (d.first - fl) : d.first, d.second); });
		break;
	}

	std::ranges::sort(dots);
	auto [r1, r2] = std::ranges::unique(dots);
	dots.erase(r1, r2);
	return dots;
}

int main(void)
{
	auto file = std::ifstream("../input_files/day13.txt");

	auto dots = read_dots(file);
	auto folds = read_folds(file);

	for (auto const& fold : folds)
	{
		dots = apply_fold(fold, std::move(dots) );
		//break;
	}

	auto dot_count = dots.size();

	std::cout << std::format("The number of dots left is {}\n", dot_count);
	return 0;
}