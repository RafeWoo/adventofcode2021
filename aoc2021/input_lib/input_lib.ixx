module;
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>

export module input_lib;


export std::string get_hello()
{
	
	return "hello_world";
}

export std::vector<int> read_input_vector(std::filesystem::path const& path)
{
    std::vector<int> result{};

    if (std::filesystem::exists(path))
    {
        auto stream = std::ifstream(path);
        int line_value = 0;
        while (stream >> line_value)
        {
            result.push_back(line_value);
        }

    }

    return result;
}
export namespace input
{
    template< typename TFunction >
    auto read_vector(std::filesystem::path const& path, TFunction t)->std::vector< std::invoke_result_t<TFunction,std::string> >
    {
        using result_type = std::invoke_result_t<TFunction,std::string>;
        std::vector< result_type > result{};

        if (std::filesystem::exists(path))
        {
            auto stream = std::ifstream(path);
            std::string line_contents;
            
            while ( std::getline(stream, line_contents) )
            {
                result.emplace_back( t(line_contents) );
            }

        }

        return result;
    }
}