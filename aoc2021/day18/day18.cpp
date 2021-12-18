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

constexpr uint8_t zero = 0;

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
			//action_taken = false;

			action_taken = exploded_something();
				action_taken = action_taken or  split_something();
			

		} while (action_taken);
	}

	uint64_t magnitude() const
	{
		auto calc_mag = CalcMagnitude{};
		uint64_t result = 3 * std::visit(calc_mag, m_left) + 2 * std::visit(calc_mag, m_right);
		
		return result;
	}

	void print(bool add_newline=true) const
	{
		std::cout << "[";

		auto v = overloaded{
			[](const uint8_t& n) { std::cout << (int) n; }
			,[](const SnailPtr& s) { s->print(false); }
		};
		std::visit(v, m_left);
		std::cout << ",";
		std::visit(v, m_right);

		
		std::cout << "]";
		if (add_newline)
		{
			std::cout << "\n";
		}
		
	}

private:

	class CalcMagnitude
	{
	public:
		uint64_t operator()(uint8_t const& n)
		{
			return n;
		}

		uint64_t operator()(SnailPtr const& s)
		{
			return s->magnitude();
		}
	};


	class Visitor
	{
	public:
		virtual ~Visitor() {};
		virtual bool visit(uint8_t&, int) = 0;
		virtual bool visit(SnailPair* pair_ptr,int) = 0;   //return true if keep iterating //false to stop
	};

	class Exploder: public Visitor
	{
	public:


		bool visit(uint8_t& n, int depth) override
		{
			switch (m_state)
			{
			case State::FindExplodingPair:
				if (depth < 5)
				{
					m_left_num = &n;
				}
				return true;

			default:
			case State::Explode:
				//should not reach this state
				//assert(false);
				return true;

			case State::FindNextNumber:
				//we've found a number to update
				n += m_right_num;
				return false; //can stop now

			}
			
		}

		bool visit(SnailPair* s, int depth) override
		{
			assert(s->m_depth == depth);

			switch (m_state)
			{
			case State::FindExplodingPair:
				if (s->m_depth >= 5)
				{
					m_state = State::Explode;					
					if (m_left_num != nullptr)
					{
						*m_left_num += std::get<0>(s->m_left);
						m_left_num = nullptr;
					}
					m_right_num = std::get<0>(s->m_right);
				}
				return true;

			case State::Explode:
				assert(s->m_depth == 4);				
				if (s->m_left.index() == 1)
				{
					s->m_left = zero;
				}
				else
				{
					s->m_right = zero;
				}
				m_state = State::FindNextNumber;
				return true;

			default:
			case State::FindNextNumber:
				//keep going
				return true;

			}

		}

		bool exploded() const
		{
			return m_state != State::FindExplodingPair;
		}


	private:


		enum class State
		{
			FindExplodingPair,    //in this state we update left num to point to the last num we see
			Explode,              //remove the last pair we visited
			FindNextNumber		  //in this state we are looking for a number to the right that we can add to	
		};

		State m_state = State::FindExplodingPair;
		uint8_t* m_left_num = nullptr;
		uint8_t  m_right_num = 0;
	
	};


	class Splitter : public Visitor
	{
	public:
		bool visit(uint8_t& n, int depth) override
		{
			bool carry_on = true;
		
			switch (m_state)
			{
			case State::FindingNumToSplit:
				if (n >= 10)
				{
					m_state = State::ReplaceWithNewPair;
					uint8_t left = n / 2;
					uint8_t right = (n + 1) / 2;
					m_new_pair = std::make_unique<SnailPair>(left,right, depth+1);  
				}
				break;

			case State::ReplaceWithNewPair:
				break;
			}

			return carry_on;
		}

		bool visit(SnailPair* s, int depth) override
		{
			bool carry_on = true;

			switch (m_state)
			{
			case State::FindingNumToSplit:			
				break;

			case State::ReplaceWithNewPair:
				assert(m_new_pair != nullptr);
				if (s->m_left.index() == 0 && std::get<0>(s->m_left) >= 10)
				{
					s->m_left = std::move(m_new_pair);
				}
				else
				{
					assert(s->m_right.index() == 0 && std::get<0>(s->m_right) >= 10);
					s->m_right = std::move(m_new_pair);
				}
				carry_on = false;
				break;
			}

			return carry_on;
		}

		bool split() const
		{
			return m_state != State::FindingNumToSplit;
		}

	private:
		enum class State
		{
			FindingNumToSplit,
			ReplaceWithNewPair
		};
		State m_state = State::FindingNumToSplit;
		SnailPtr m_new_pair = nullptr;
	};


	class Walker
	{
	public:
		Walker(Visitor& v, int depth)
			: m_v{v}
			, m_depth{depth}
		{}

		bool operator()(uint8_t& n) 
		{
			return m_v.visit(n, m_depth);			
		}

		bool operator()(SnailPtr& s)
		{
			return SnailPair::walk_tree(s.get(), m_v, m_depth + 1);			
		}
	private:
		Visitor& m_v;
		int m_depth;
	};
	
	static bool walk_tree(SnailPair* pair, Visitor& v, int depth)
	{
		auto walker = Walker{v, depth};
	
		
		if (!std::visit(walker, pair->m_left))
		{
			return false;
		}	
		
		if (!v.visit(pair, depth))
		{
			return false;
		}

		if (!std::visit(walker, pair->m_right))
		{
			return false;
		}

		return true;
	}


	bool exploded_something()
	{
		Exploder exploder{};
		walk_tree(this, exploder, 1);
		return exploder.exploded();
	}


	bool split_something()
	{
		Splitter splitter{};
		walk_tree(this, splitter, 1);
		return splitter.split();		
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
	int m_depth = 1;
};

SnailPtr add_snails(SnailPtr left, SnailPtr right)
{
	if (!left) return right;
	if (!right) return left;

	auto result = std::make_unique<SnailPair>(std::move(left), std::move(right));
	result->reduce();
	return result;
}


SnailPtr string_to_snail_pair(std::string const& s)
{
	std::stack<Element> elements{};
	int depth = 0;
	
	uint8_t cur_val = 0;
	bool reading_val = false;

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
			if (reading_val)
			{
				elements.push(cur_val);
				reading_val = false;
			}

			assert(elements.size() >= 2);
			auto right = std::move(elements.top());
			elements.pop();
			auto left = std::move(elements.top());
			elements.pop();

			elements.push(std::make_unique<SnailPair>(std::move(left), std::move(right), depth));
			--depth;
		}
			break;

		default:
			//do nothing
			break;

		case ',':
			if (reading_val)
			{
				elements.push(cur_val);
				reading_val = false;
			}
			break;

		 //must be a number
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			{
				if (!reading_val)
				{
					cur_val = 0;
					reading_val = true;
				}
			uint8_t val = c - '0';
			//elements.push(val);
			cur_val = 10 * cur_val + val;
			}
			break;
		}
	}

	//at end, stack should contain 1 Element which should be a SnailPtr
	assert(elements.size() == 1);
	assert( elements.top().index() ==1  );
	return std::get<1>(std::move(elements.top()));
}

int main(void)
{
	auto test = string_to_snail_pair("[[[[[9,8],1],2],3],4]");

	auto test1 = string_to_snail_pair("[1, 2]");
	auto test2 = string_to_snail_pair("[[3,4],5]");

	auto r = add_snails(std::move(test1), std::move(test2));
	auto mag = r->magnitude();

	auto test3 = string_to_snail_pair("[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]");
	auto mag3 = test3->magnitude();


	auto test4 = string_to_snail_pair("[[[[[9,8],1],2],3],4]");
	test4->reduce();
	test4->print();

	auto test5 = string_to_snail_pair("[10, 1]");
	test5->print();
	test5->reduce();
	test5->print();

	//auto snails = input::read_vector("../input_files/day18.txt", string_to_snail_pair);
	//accumulate
	//get magnitude
	return 0;
} 