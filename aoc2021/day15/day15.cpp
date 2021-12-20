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
#include <queue>
#include <optional>
#include <limits>

import input_lib;



using Grid = std::array<int8_t, 10000>;

constexpr size_t xy(size_t x, size_t y)
{
    return y * 100 + x;
}


using PosAndCost = std::pair<size_t, int32_t>;

struct CompareCost
{
    constexpr bool operator()(PosAndCost const& lhs, PosAndCost const& rhs)
    {
        return lhs.second > rhs.second;
    }
};

std::vector<size_t> gen_neighbours(size_t pos)
{
    std::vector<size_t> neighbours;
    neighbours.reserve(4);

    //up,down,left right
    if (pos >= 100)
    {
        neighbours.push_back(pos-100);
    }
    if (pos < 9900)
    {
        neighbours.push_back(pos + 100);
    }
    if ((pos % 100) != 0)
    {
        neighbours.push_back(pos - 1);
    }
    if ((pos % 100) != 99)
    {
        neighbours.push_back(pos + 1);
    }
    return neighbours;
}

class PriorityQueue : public std::priority_queue< PosAndCost, std::vector<PosAndCost>, CompareCost >
{
public:

    bool contains(size_t pos) const
    {
        auto it = std::ranges::find_if(c, [pos](auto const& pandc) {return pandc.first == pos; });
        return it != c.end();
    }
};

std::deque<size_t> reconstruct_path(std::map<size_t, size_t> const& came_from, size_t current)
{
    std::deque<size_t> path = { current };
    
    for (auto it = came_from.find(current); it != came_from.end(); it = came_from.find(it->second))
    {
        path.push_front(it->second);
    }

    return path;
}

constexpr int32_t h_cost(size_t pos)
{
    //use manhatten distance //admissable cost is at least 1 at every step
    return static_cast<int32_t>( 99 - (pos % 100) + 99 - (pos / 100));
}

std::optional<std::deque<size_t>> search_grid(Grid const& grid)
{
    size_t start_pos = xy(0,0);
    size_t dest_pos = xy(99, 99);



    PriorityQueue open_list{};
    open_list.emplace(start_pos, 0);

    std::map<size_t, size_t> came_from_map{};

    std::vector<size_t> closed_list{ };


    std::array<int32_t, 10000> g_score;
    std::ranges::fill(g_score, std::numeric_limits<int32_t>::max());
    g_score[start_pos] = 0;
#if 0
    std::array<int32_t, 10000> f_score;
    std::ranges::fill(f_score, std::numeric_limits<int32_t>::max());
    f_score[start_pos] = h_cost(start_pos);
#endif

    while (!open_list.empty())
    {
        auto [current,q_cost] = open_list.top();
        open_list.pop();


        if (current == dest_pos)
        {
            return reconstruct_path(came_from_map, current);
        }

        auto neighbours = gen_neighbours(current);

        for (auto n : neighbours)
        {                                                //cost of current->n is the grid score of n
            auto tentative_g_score = g_score[current] + grid[n];

            if (tentative_g_score < g_score[n])
            {
                came_from_map[n] = current;
                g_score[n] = tentative_g_score;
                auto f_score = tentative_g_score + h_cost(n);

                if( !open_list.contains( n ) )// neighbor not in openSet
                {
                    open_list.emplace(n, f_score);
                }
            }
        }        
    }
    
    return std::nullopt;
}

Grid make_grid(std::vector<std::string>&& lines)
{
    Grid grid;
    size_t pos = 0;
    for (auto const& l : lines)
    {
        for (auto c : l)
        {
            grid[pos++] = c - '0';
        }
    }

    return grid;
}


int main(void)
{
    auto lines = input::read_vector("../input_files/day15.txt", [](auto const& s) {return s; });
    
    auto grid = make_grid(std::move(lines));

    auto result = search_grid(grid);

    if (result)
    {

        auto path_cost = std::accumulate(result->begin(), result->end(), static_cast<int32_t>( - 1 * grid[0]), [&grid](int32_t a, size_t pos)
            {
                return a + grid[pos];
            }
        );

        std::cout << std::format("the safest pash has risk {}\n", path_cost);

    }

	return 0;
}

