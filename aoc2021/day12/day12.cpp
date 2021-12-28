
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <regex>
#include <cctype>
#include <assert.h>
#include <format>
#include <optional>
#include <stack>


import input_lib;



enum class CaveSize
{
	Big,
	Little,
	Terminal	
};

using CaveId = size_t;  //string hash

CaveId operator "" _cid(const char* s, size_t len)
{
	return std::hash<std::string>{}(std::string{ s,len });
}

class Cave
{
public:
	Cave(const std::string& s)
		: m_size{ ::isupper(s[0]) ? CaveSize::Big : CaveSize::Little }
		, m_id{ std::hash<std::string>{}(s) }
	{
		if (m_id == "start"_cid or
			m_id == "end"_cid)
		{
			m_size = CaveSize::Terminal;
		}
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



	std::vector<CaveId> connections(CaveId c, std::unordered_map<CaveId, bool> const& visited, std::optional<CaveId>& twice_cave)
	{
		std::vector<CaveId> to_visit{};


		for (auto const& connecting_cave : m_connections[c])
		{

			switch (connecting_cave.size())
			{
			case CaveSize::Big:
				to_visit.push_back(connecting_cave.id());				
				break;

			case CaveSize::Terminal:
				if ( auto it = visited.find(connecting_cave.id()); it == visited.end() or !it->second)
				{
					to_visit.push_back(connecting_cave.id());				
				}
				break;


			case CaveSize::Little:
				if ( auto it = visited.find(connecting_cave.id()); it == visited.end() or !it->second)
				{
					to_visit.push_back(connecting_cave.id());					
				}
				else
				{
					//have we visited a small cave twice yet?
					//if not then allow it
					if (!twice_cave)
					{
						twice_cave = connecting_cave.id();
						to_visit.push_back(connecting_cave.id());
					}
				}
				break;

			}

		}

		return to_visit;
	}

	std::vector<Path> find_all_paths_between(CaveId start, CaveId end)
	{		
		std::vector<Path> paths{};

		std::unordered_map<CaveId, bool> visited{};
		
		std::optional<CaveId> twice_cave{};
		Path current_path{};
		current_path.push_back(start);
		visited[start] = true;
		
		std::vector< std::vector<CaveId> > visit_stack;
		visit_stack.emplace_back(connections(start, visited, twice_cave));
		

		while ( !current_path.empty() )
		{

			auto to_visit = std::move(visit_stack.back());
			visit_stack.pop_back();


			if (to_visit.empty())
			{	
				visited[current_path.back()] = false;
				current_path.pop_back();
				continue;
			}


			//add the top element from the to_visit stack to the path
			auto next = to_visit.back();
			to_visit.pop_back();


			current_path.push_back(next);
			visited[next] = true;

			//push the to_visit back on the stack
			visit_stack.emplace_back(std::move(to_visit));
								
			if (next == end)
			{
				paths.push_back(current_path);

				visited[next] = false;
				current_path.pop_back();
			}
			else
			{
				//push the connections from the top element on the stack
				visit_stack.emplace_back(connections(next, visited, twice_cave));

			}						
		}

		return paths;
	}

private:


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




int main(void)
{
	auto edges = input::read_vector("../input_files/day12_example.txt", string_to_edge);

	CaveGraph cave_graph{ edges };

	auto paths = cave_graph.find_all_paths_between( "start"_cid,"end"_cid);

	auto path_count = paths.size();

	std::cout << std::format("The number of paths is {}\n", path_count);

	return 0;
}