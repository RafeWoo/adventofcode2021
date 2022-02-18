#include <iostream>
#include <regex>
#include <ranges>
#include <format>
#include <bitset>
#include <memory>
#include <tuple>

import input_lib;


class Cuboid
{
public:
	Cuboid() = default;

	Cuboid(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
		: m_x_min{ x_min }
		, m_x_max{ x_max }
		, m_y_min{ y_min }
		, m_y_max{ y_max }
		, m_z_min{ z_min }
		, m_z_max{ z_max }
	{}


	size_t volume() const
	{
		return (m_x_max - m_x_min) *
			(m_y_max - m_y_min) *
			(m_z_max - m_z_min);
	}

	void reduce_to(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
	{
		if (m_x_max < x_min ||
			m_x_min > x_max)
		{
			m_x_max = 0;
			m_x_min = 0;
		}
		else
		{
			m_x_min = std::max(m_x_min, x_min);
			m_x_max = std::min(m_x_max, x_max);
		}
		if (m_y_max < y_min ||
			m_y_min > y_max)
		{
			m_y_max = 0;
			m_y_min = 0;
		}
		else
		{
			m_y_min = std::max(m_y_min, y_min);
			m_y_max = std::min(m_y_max, y_max);
		}
		if (m_z_max < z_min ||
			m_z_min > z_max)
		{
			m_z_max = 0;
			m_z_min = 0;
		}
		else
		{
			m_z_min = std::max(m_z_min, z_min);
			m_z_max = std::min(m_z_max, z_max);
		}

	}


	struct Iterator
	{
		int m_x;		
		int m_y;
		int m_z;
		const Cuboid* m_cuboid;

		Iterator& operator++()
		{
			if (m_z < m_cuboid->m_z_max)
			{
				if (++m_x == m_cuboid->m_x_max)
				{
					m_x = m_cuboid->m_x_min;
					if (++m_y == m_cuboid->m_y_max)
					{
						m_y = m_cuboid->m_y_min;
						++m_z;
					}
				}
			}
			return *this;		
		}

		std::tuple<int, int, int> operator*()
		{
			return { m_x, m_y, m_z };
		}

		friend bool operator ==(Iterator const& lhs, Iterator const& rhs)
		{
			return lhs.m_x == rhs.m_x &&
				lhs.m_y == rhs.m_y &&
				lhs.m_z == rhs.m_z &&
				lhs.m_cuboid == rhs.m_cuboid;
		}

		friend bool operator !=(Iterator const& lhs, Iterator const& rhs)
		{
			return !(lhs == rhs);
		}
	};

	Iterator begin() const
	{
		return { m_x_min , m_y_min, m_z_min, this };
	}

	Iterator end() const
	{
		return { m_x_min , m_y_min, m_z_max, this };
	}


private:
	int m_x_min;
	int m_x_max;
	int m_y_min;
	int m_y_max;
	int m_z_min;
	int m_z_max;
};

class RebootStep
{

public:
	RebootStep() = default;

	RebootStep(bool on, int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
		: m_on{ on }
		, m_cuboid{ x_min, x_max, y_min, y_max, z_min, z_max }	
	{

	}


	Cuboid const& cuboid() const
	{
		return m_cuboid;
	}

	Cuboid& cuboid()
	{
		return m_cuboid;
	}

	bool is_on() const
	{
		return m_on;
	}
private:
	bool m_on;
	Cuboid m_cuboid;
};

class Reactor final
{
public:
	Reactor()
		: m_cubes{ std::make_unique<std::bitset<ReactorSize>>() }
	{

	}

	static size_t to_reactor_index(std::tuple<int, int, int> const& c)
	{
		//shift each value  by adding 50
		size_t index = (std::get<0>(c) + 50) + ((std::get<1>(c) + 50) * 101) + ((std::get<2>(c) + 50) * 101 * 101);
		return index;
	}

	void apply_reboot_step(RebootStep const& step)
	{
		if (step.is_on())
		{
			for (auto coords : step.cuboid() )
			{
				m_cubes->set(to_reactor_index(coords) );
			}
		}
		else
		{
			for (auto coords : step.cuboid())
			{
				m_cubes->reset(to_reactor_index(coords));
			}
		}
	}

	size_t cube_count() const
	{
		return m_cubes->count();
	}

private:

	static constexpr size_t ReactorSize = 101 * 101 * 101;
	std::unique_ptr<std::bitset<ReactorSize>> m_cubes;
};


RebootStep string_to_reboot_step(std::string const& s)
{

	auto pattern = std::regex(R"(^(on|off) x=([\-0-9]*)\.\.([\-0-9]*),y=([\-0-9]*)\.\.([\-0-9]*),z=([\-0-9]*)\.\.([\-0-9]*)$)");
	std::smatch results;
	if (std::regex_match(s, results, pattern))
	{
		bool is_on = results[1] == "on";
		int x_min = std::stoi(results[2]);
		int x_max = std::stoi(results[3])+1;
		int y_min = std::stoi(results[4]);
		int y_max = std::stoi(results[5])+1;
		int z_min = std::stoi(results[6]);
		int z_max = std::stoi(results[7])+1;
		return RebootStep(is_on, x_min, x_max, y_min, y_max, z_min, z_max);
	}
	return RebootStep{};
}

RebootStep reduce_step(RebootStep step)
{
	step.cuboid().reduce_to(-50, 50, -50, 50, -50, 50);
	return step;
}

bool is_valid_step(RebootStep const step)
{
	return step.cuboid().volume() > 0;
}

int main(void)
{
	
	auto steps = input::read_vector("../input_files/day22.txt", string_to_reboot_step);

	auto init_steps = steps | std::views::transform(reduce_step) | std::views::filter(is_valid_step);
	int i = 0;

	Reactor reactor{};
	for (auto const& s : init_steps)
	{
		//std::cout << std::format("{}) vol = {}\n", i++, s.volume());
		reactor.apply_reboot_step(s);
	}

	std::cout << std::format("The number of cubes is {}\n", reactor.cube_count());

	return 0;
}
