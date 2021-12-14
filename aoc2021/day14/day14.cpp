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


#if 0
//part 1
using ElementMap = std::map<std::pair<char,char>, char>;

ElementMap create_element_map(std::istream& s)
{
	ElementMap map{};

	std::regex element_mapping("^([A-Z])([A-Z]) -> ([A-Z])$");
	std::string line;
	while (std::getline(s, line))
	{
		std::smatch matches;
		if (std::regex_match(line, matches, element_mapping) )
		{
			std::string const& one = matches[1];
			std::string const& two = matches[2];
			std::string const& thr = matches[3];

			map.emplace( std::make_pair(one[0], two[0]), thr[0]);
		}
	}

	return map;
}

std::string process_elements(std::string&& elements, ElementMap const& map)
{

	auto it1 = elements.begin();
	auto it2 = it1+1;
	for (; it2 != elements.end(); )
	{
		auto find_it = map.find(std::make_pair(*it1, *it2));
		char new_element = find_it->second;
	
		it1 = elements.insert(it2, new_element) + 1;
		
		
		it2 = it1 + 1;

		int ten = 9;
		++ten;
	}

	return elements;
}

int main(void)
{
	auto file = std::ifstream("../input_files/day14.txt");

	std::string elements;
	std::getline(file, elements);

	std::string dummy;
	std::getline(file, dummy);
		
	auto element_map = create_element_map(file);

	for (int step = 0; step < 10; ++step)
	{
		elements = process_elements(std::move(elements), element_map);
	}

	std::map<char,int> element_counts{};

	std::ranges::for_each(elements, [&element_counts](auto e)
		{
			element_counts[e]++;
		}
	);

	

	auto [min,max] = std::ranges::minmax_element(element_counts, std::less(), [](const auto& p) {return p.second; });
	auto diff = max->second - min->second;

	std::cout << std::format("The diff between the max {} and the min {} is {}\n", max->second, min->second, diff);

	return 0;
}

#else 

//part 2

using PairCounts = std::map<std::pair<char,char>, uint64_t > ;    
using ElementCounts = std::map<char, uint64_t>;
using ElementMap = std::map<std::pair<char, char>, char>;

ElementMap create_element_map(std::istream& s)
{
	ElementMap map{};

	std::regex element_mapping("^([A-Z])([A-Z]) -> ([A-Z])$");
	std::string line;
	while (std::getline(s, line))
	{
		std::smatch matches;
		if (std::regex_match(line, matches, element_mapping))
		{
			std::string const& one = matches[1];
			std::string const& two = matches[2];
			std::string const& thr = matches[3];

			map.emplace(std::make_pair(one[0], two[0]), thr[0]);
		}
	}

	return map;
}

PairCounts process_elements(PairCounts&& pair_counts, ElementCounts& element_counts, ElementMap const& map)
{
	PairCounts next_counts{};

	for (auto [pair,count] : pair_counts)
	{
		//find pair in the map
		auto it = map.find(pair);

		//increase the element count by the pair count
		element_counts[it->second] += count;

		//increase each new pair by the pair_count
		next_counts[std::make_pair(pair.first, it->second)] += count;
		next_counts[std::make_pair(it->second, pair.second)] += count;
	}
	
	return next_counts;
	
}

int main(void)
{
	auto file = std::ifstream("../input_files/day14.txt");

	std::string elements;
	std::getline(file, elements);

	std::string dummy;
	std::getline(file, dummy);

	auto element_map = create_element_map(file);

	ElementCounts element_counts{};
	std::ranges::for_each(elements, [&element_counts](auto e)
		{
			element_counts[e]++;
		}
	);

	PairCounts pair_counts{};
	{
		auto it1 = elements.begin();
		auto it2 = it1 + 1;
		for (; it2 != elements.end(); ++it1, ++it2)
		{
			pair_counts[std::make_pair(*it1, *it2)]++;					
		}
	}


	for (int step = 0; step < 40; ++step)
	{
		pair_counts = process_elements( std::move(pair_counts), element_counts, element_map);
	}


	auto [min, max] = std::ranges::minmax_element(element_counts, std::less(), [](const auto& p) {return p.second; });
	auto diff = max->second - min->second;

	std::cout << std::format("The diff between the max {} and the min {} is {}\n", max->second, min->second, diff);

	return 0;
}



#endif