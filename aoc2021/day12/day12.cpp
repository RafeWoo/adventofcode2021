
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <regex>
#include <cctype>
#include <assert.h>
#include <format>

import input_lib;



enum class CaveSize
{
	Big,
	Little
};

using CaveId = size_t;  //string hash

class Cave
{
public:
	Cave(const std::string& s)
		: m_size{ ::isupper(s[0]) ? CaveSize::Big : CaveSize::Little }
		, m_id{ std::hash<std::string>{}(s) }
	{
	
	}

	Cave()
		: m_size(CaveSize::Little)
		, m_id(0)
	{}


	CaveSize size() const
	{
		return m_size;
	}

	CaveId id() const
	{
		return m_id;
	}

private:
	CaveSize m_size;
	CaveId m_id;
};


using Edge = std::pair<Cave, Cave>;
using Path = std::vector<CaveId>;

class CaveGraph
{

public:
	CaveGraph( std::vector<Edge> const& edges)
	{
		for (auto const& e : edges)
		{
			m_connections[e.first.id()].push_back(e.second);
			m_connections[e.second.id()].push_back(e.first);
		}

	}


	std::vector<Path> find_all_paths_between(CaveId start, CaveId end)
	{
		std::vector<Path> paths{};

		std::unordered_map<CaveId, bool> visited{};
		
		Path path{};
		find_all_paths(start, end, paths, visited, path);

		return paths;
	}

private:


	void find_all_paths(CaveId start, CaveId end, std::vector<Path>& paths, std::unordered_map<CaveId, bool>& visited, Path& current_path)
	{
		visited[start] = true;

		current_path.push_back(start);

		if (start == end)
		{
			paths.push_back(current_path);
		}
		else
		{
			for (auto const& connecting_cave : m_connections[start])
			{
				if (!visited[connecting_cave.id()] or connecting_cave.size() == CaveSize::Big)
				{
					find_all_paths(connecting_cave.id(), end, paths, visited, current_path);
				}
			}
		}

		current_path.pop_back();
		visited[start] = false;
	}

	std::unordered_map<CaveId, std::vector<Cave> > m_connections;
	//for each vertex we have a list of edges to other vertices

	//string ids a vertex

};



Edge string_to_edge(std::string const& s)
{
	std::hash<std::string> hasher{};
	auto pattern = std::regex("^([a-zA-Z]+)-([a-zA-Z]+)$");
	std::smatch results;
	if (std::regex_match(s, results, pattern))
	{			
		return std::make_pair(Cave{ results[1] }, Cave{ results[2] });
	}
	
	assert(false);
	return std::make_pair(Cave{}, Cave{});
}


CaveId operator "" _cid(const char* s, size_t len)
{
	return std::hash<std::string>{}(std::string{ s,len });
}

int main(void)
{
	auto edges = input::read_vector("../input_files/day12.txt", string_to_edge);

	CaveGraph cave_graph{ edges };

	auto paths = cave_graph.find_all_paths_between( "start"_cid,"end"_cid);

	auto path_count = paths.size();

	std::cout << std::format("The number of paths is {}\n", path_count);

	return 0;
}