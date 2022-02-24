#include <algorithm>
#include <iostream>
#include <format>
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <optional>


import input_lib;


auto identity( std::string const& io)
{
    return  io;
}


enum class Blah: uint8_t
{
    Empty,
    East,
    South
};

class Map
{
public:

    static size_t index( size_t x, size_t y )
    {
        return (y % HEIGHT)* WIDTH + (x % WIDTH);
    }

    explicit Map( std::vector<std::string> const& rows )
        : Map()
    {

        size_t row = 0;
       
        for( auto const& row_string : rows )
        {
            size_t col = 0;
            for( auto c : row_string )
            {
                switch( c )
                { 
                    case '>':
                    m_map[index( col, row )] = Blah::East;
                    break;

                    case 'v':
                    m_map[index( col, row )] = Blah::South;
                    break;

                    default:
                    break;
                }
                ++col;
            }
            ++row;
        }
    }
    Map()
        : m_map( WIDTH* HEIGHT, Blah::Empty )
    {}

    friend void swap( Map& lhs, Map& rhs )
    {
        using std::swap;
        swap( lhs.m_map, rhs.m_map );
    }

    friend bool operator==( Map const& lhs, Map const& rhs )
    {
        return lhs.m_map == rhs.m_map;
    }

    static void update_map( Map const& src, Map& dst )
    {
        static  std::vector<Blah> work(WIDTH*HEIGHT, Blah::Empty);

        //step move easters
        for( size_t row = 0; row < HEIGHT; ++row )
        {
            for( size_t col = 0 ; col < WIDTH; ++col)
            { 
                work[index( col, row )] = east_update(src.m_map, col, row);
            }
        }
        
        //step move southers
        for( size_t row = 0; row < HEIGHT; ++row )
        {
            for( size_t col = 0; col < WIDTH; ++col )
            {
                dst.m_map[index( col, row )] = south_update( work, col, row );
            }
        }
    }
private:

    static Blah east_update( std::vector<Blah> const& map, size_t x, size_t y )
    {
        Blah result;
        switch( map[index( x, y )] )
        {
            case Blah::East:
            //empty if right neighbour empty
            result = map[index( x + 1, y )] == Blah::Empty ? Blah::Empty : Blah::East;
            //else East
            break;

            case Blah::Empty:
            //east if left neighbour east
            result = map[index( x + WIDTH - 1, y )] == Blah::East ? Blah::East : Blah::Empty;
            //else empty
            break;

            case Blah::South:
            result = Blah::South;
            break;
        }

        return result;
    }
    static Blah south_update( std::vector<Blah> const& map, size_t x, size_t y )
    {
        Blah result;
        switch( map[index( x, y )] )
        {
            case Blah::East:
            result = Blah::East;
            break;
       
            case Blah::Empty:
            //south if up neighbour south
            result = map[index( x, y + HEIGHT - 1)] == Blah::South ? Blah::South : Blah::Empty;
            //else empty
            break;

            case Blah::South:
            result= map[index( x, y + 1 )] == Blah::Empty ? Blah::Empty : Blah::South;
            break;
        }

        return result;
    }

#if 0
    static constexpr size_t WIDTH = 139;
    static constexpr size_t HEIGHT = 137;
#else
    static constexpr size_t WIDTH = 10;
    static constexpr size_t HEIGHT = 9;
#endif

    std::vector<Blah> m_map;
};






int main()
{
    auto rows = input::read_vector( "../input_files/day25_example.txt", identity );

    //create map
    Map map{ rows };
    Map new_map{};

    size_t step_count = 0;

    while( step_count < 2000000 )
    {        
        Map::update_map( map, new_map);

        ++step_count;

        if( new_map == map )
        {
            break;
        }

        swap( map, new_map );
       
    }

    std::cout << std::format( "The cumbers stopped moving on step {}\n", step_count );

    return 0;
}