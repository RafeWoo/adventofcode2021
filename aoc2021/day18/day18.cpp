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
#include <variant>

import input_lib;


// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;


class SnailPair;
using SnailPtr = std::unique_ptr<SnailPair>;

using Element = std::variant<uint8_t, SnailPtr>;

class SnailPair
{
public:
	SnailPair(uint8_t left, uint8_t right)
		: m_left{ left }
		, m_right{ right }
	{
		
	}

	SnailPair(uint8_t left, SnailPtr right)
		: m_left{ left }
		, m_right{ std::move(right) }
	{	
		update_child_depths();
	}

	SnailPair(SnailPtr left, uint8_t right)
		: m_left{ std::move(left) }
		, m_right{ right }
	{		
		update_child_depths();
	}

	SnailPair(SnailPtr left, SnailPtr right)
		: m_left{ std::move(left) }
		, m_right{ std::move(right) }
	{
		update_child_depths();
	}

	SnailPair( Element&& left, Element&& right, int depth)
		: m_left{ std::move(left) }
		, m_right{ std::move(right) }
		, m_depth{depth}
	{
	
	}

	void reduce()
	{
		bool action_taken = false;
		do
		{
			auto blah = find_exploding_child();

			if (blah)
			{
				explode(blah);
				action_taken = true;
			}
			else
			{
				auto bloop = find_big_number();
				if (bloop)
				{
					split();
					action_taken = true;
				}
			}

		} while (action_taken);
	}

private:

	bool find_exploding_child()
	{
		return false;
	}


	void explode(bool)
	{

	}

	bool find_big_number()
	{
		return false;
	}

	void split()
	{

	}

	class DepthUpdater
	{
	public:
		void operator()(uint8_t const& n) {};
		void operator()(SnailPtr const& s)
		{
			s->inc_depth();
			s->update_child_depths();
		}
	};

	void update_child_depths()
	{
		auto du = DepthUpdater{};

		std::visit(du, m_left);
		std::visit(du, m_right);

	}

	void inc_depth()
	{
		++m_depth;
	}

	Element m_left;
	Element m_right;
	int m_depth = 0;
};

SnailPtr string_to_snail_pair(std::string const& s)
{
	std::stack<Element> elements{};
	int depth = 0;
	for (char c : s)
	{
		switch (c)
		{
		case '[': //depth increase by 1
			++depth;
			break;

		case ']':
			//pop 2 elements off stack //right, left
			//create a pair and push onto the stack
			//depth decreases by 1
		{
			assert(elements.size() >= 2);
			auto right = std::move(elements.top());
			elements.pop();
			auto left = std::move(elements.top());
			elements.pop();

			elements.push(std::make_unique<SnailPair>(std::move(left), std::move(right), depth));
			--depth;
		}
			break;

		case ',': //do nothing
			break;

		default: //must be a number
			{
			uint8_t val = c - '0';
			elements.push(val);
			}
			break;
		}
	}

	//at stack should contain 1 Element which should be a SnailPtr
	assert(elements.size() == 1);
	assert( elements.top().index() ==1  );
	return std::get<1>(std::move(elements.top()));
}

int main(void)
{
	auto test = string_to_snail_pair("[[[[[9,8],1],2],3],4]");

	auto snails = input::read_vector("../input_files/day18.txt", string_to_snail_pair);

	return 0;
} 