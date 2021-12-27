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

template<size_t GRID_SIZE>
constexpr size_t xy(size_t x, size_t y)
{
    return y * GRID_SIZE + x;
}





using PosAndCost = std::pair<size_t, int32_t>;

struct CompareCost
{
    constexpr bool operator()(PosAndCost const& lhs, PosAndCost const& rhs)
    {
        return lhs.second > rhs.second;
    }
};

template <size_t GRID_SIZE>
std::vector<size_t> gen_neighbours(size_t pos)
{
    std::vector<size_t> neighbours;
    neighbours.reserve(4);

    //up,down,left right
    if (pos >= GRID_SIZE) //not on top row , can move up
    {
        neighbours.push_back(pos- GRID_SIZE);
    }
    constexpr size_t FIRST_OF_LAST_ROW = xy<GRID_SIZE>(GRID_SIZE - 1, GRID_SIZE - 2) + 1;
    if (pos < FIRST_OF_LAST_ROW) //not on the bottom row, can move down
    {
        neighbours.push_back(pos + GRID_SIZE);
    }
    if ((pos % GRID_SIZE) != 0)
    {
        neighbours.push_back(pos - 1);
    }
    if ((pos % GRID_SIZE) != (GRID_SIZE-1))
    {
        neighbours.push_back(pos + 1);
    }
    return neighbours;
}


int8_t grid_score(Grid const& grid, size_t pos)
{
    size_t xpos = pos % 500;
    size_t ypos = pos / 500;
    size_t gx = xpos % 100;
    size_t gy = ypos % 100;

    int32_t original_score = grid[ xy<100>(gx,gy) ];
    size_t tile_x = xpos / 100;
    size_t tile_y = ypos / 100;


    return ((original_score + tile_x + tile_y -1) % 9) + 1;
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

template <size_t GRID_SIZE>
constexpr int32_t h_cost(size_t pos)
{
    //use manhatten distance from goal //admissable since cost is at least 1 at every step
    return static_cast<int32_t>( 2 * GRID_SIZE - 2 - (pos % GRID_SIZE)  - (pos / GRID_SIZE));
}

std::optional<std::deque<size_t>> search_grid(Grid const& grid)
{
    constexpr size_t PART1_GRIDSIZE = 100;

    size_t start_pos = xy<PART1_GRIDSIZE>(0,0);
    size_t dest_pos = xy<PART1_GRIDSIZE>(PART1_GRIDSIZE-1, PART1_GRIDSIZE-1);



    PriorityQueue open_list{};
    open_list.emplace(start_pos, 0);

    std::map<size_t, size_t> came_from_map{};

    std::vector<size_t> closed_list{ };


    std::map< size_t, int32_t> g_score;   
    g_score[start_pos] = 0;


    while (!open_list.empty())
    {
        auto [current,q_cost] = open_list.top();
        open_list.pop();


        if (current == dest_pos)
        {
            return reconstruct_path(came_from_map, current);
        }

        auto neighbours = gen_neighbours< PART1_GRIDSIZE>(current);

        for (auto n : neighbours)
        {                                                //cost of current->n is the grid score of n
            auto tentative_g_score = g_score[current] + grid[n];

            if (tentative_g_score < g_score[n])
            {
                came_from_map[n] = current;
                g_score[n] = tentative_g_score;
                auto f_score = tentative_g_score + h_cost< PART1_GRIDSIZE>(n);

                if( !open_list.contains( n ) )// neighbor not in openSet
                {
                    open_list.emplace(n, f_score);
                }
            }
        }        
    }
    
    return std::nullopt;
}



std::optional<std::deque<size_t>> search_grid2(Grid const& grid)
{
    constexpr size_t PART2_GRIDSIZE = 500;

    size_t start_pos = xy<PART2_GRIDSIZE>(0, 0);
    size_t dest_pos = xy< PART2_GRIDSIZE>(PART2_GRIDSIZE-1, PART2_GRIDSIZE-1);



    PriorityQueue open_list{};
    open_list.emplace(start_pos, 0);

    std::map<size_t, size_t> came_from_map{};


    std::map<size_t, int32_t> g_score;   
    g_score[start_pos] = 0;


    while (!open_list.empty())
    {
        auto [current, q_cost] = open_list.top();
        open_list.pop();


        if (current == dest_pos)
        {
            return reconstruct_path(came_from_map, current);
        }

        auto neighbours = gen_neighbours< PART2_GRIDSIZE>(current);

        for (auto n : neighbours)
        {   
            auto g_it = g_score.find(n);
            auto n_gscore = g_it != g_score.end() ? g_it->second : std::numeric_limits<size_t>::max();

            //cost of current->n is the grid score of n
            auto tentative_g_score = g_score[current] + grid_score(grid,n);

            if (tentative_g_score < n_gscore)
            {
                came_from_map[n] = current;
                g_score[n] = tentative_g_score;
                auto f_score = tentative_g_score + h_cost< PART2_GRIDSIZE>(n);

                if (!open_list.contains(n))// neighbor not in openSet
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

#if 1
int main(void)
{
    auto lines = input::read_vector("../input_files/day15.txt", [](auto const& s) {return s; });

    auto grid = make_grid(std::move(lines));

    auto result = search_grid2(grid);
   

    if (result)
    {

        auto path_cost = std::accumulate(result->begin(), result->end(), static_cast<int32_t>(-1 * grid[0]), 
            [&grid](int32_t a, size_t pos)
            {
                return a + grid_score(grid,pos);
            }
        );

        std::cout << std::format("the safest path has risk {}\n", path_cost);

    }

    return 0;
}
#else
//part 0
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

        std::cout << std::format("the safest path has risk {}\n", path_cost);

    }

	return 0;
}
#endif
