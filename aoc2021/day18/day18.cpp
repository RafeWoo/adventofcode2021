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

using Number = int8_t;
using Element = std::variant<Number, SnailPtr>;

constexpr Number zero = 0;
constexpr Number ExplodingPlaceHolder= -1;

class SnailPair
{
public:
#if 0
	SnailPair(Number left, Number right)
		: m_left{ left }
		, m_right{ right }
	{
		
	}

	SnailPair(Number left, SnailPtr right)
		: m_left{ left }
		, m_right{ std::move(right) }
	{	
		update_child_depths();
	}

	SnailPair(SnailPtr left, Number right)
		: m_left{ std::move(left) }
		, m_right{ right }
	{		
		update_child_depths();
	}
#endif
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

#if 0
			action_taken = false;

			if (exploded_something())
			{
				std::cout << "exploded\n";
				action_taken = true;
				print();

			}
			else if (split_something())
			{
				std::cout << "split\n";
				action_taken = true;
				print();
			}

#else
			action_taken = exploded_something() or split_something();
#endif
		
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
			[](const Number& n) { std::cout << (int) n; }
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
		uint64_t operator()(Number const& n)
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
		virtual bool visit(Number&, int, SnailPair*) = 0;
		virtual bool visit(SnailPair* pair_ptr,int, SnailPair* ) = 0;   //return true if keep iterating //false to stop
	};

	class Exploder: public Visitor
	{
	public:

		bool visit(Number& n, int depth, SnailPair* parent) override
		{
			bool keep_going = true;

			switch (m_state)
			{
			case State::FindExplodingPair:
				//record the right-most number to the left of exploding pair
				if (depth < 5)
				{
					m_left_num = &n;
				}
				break;

			case State::FindPlaceHolder:
				if (n == ExplodingPlaceHolder)
				{
					n = zero;
					m_state = State::FindNextNumber;
				}
				break;

			case State::FindNextNumber:
				//we've found a number to update
				n += m_right_num;
				m_state = State::Finished;
				keep_going = false;
				break;

			case State::Finished:
				keep_going = false;
				break;
			
			}

			return keep_going;
			
		}

		bool visit(SnailPair* s, int depth, SnailPair* parent) override
		{
			assert(s->m_depth == depth);

			bool keep_going = true;

			switch (m_state)
			{
			case State::FindExplodingPair:
				if (s->m_depth == 4)
				{
					if (explode_either_child(s))
					{
						m_state = State::FindPlaceHolder;
						keep_going = false;
					}			     
					
				}
				break;

		
			case State::FindPlaceHolder:
				break;

			case State::FindNextNumber:
				break;

			case State::Finished:
				keep_going = false;
				break;

			}

			return keep_going;
		}

		bool exploded() const
		{
			return m_state != State::FindExplodingPair;
		}


	private:

		bool explode_either_child(SnailPair* s)
		{
			bool exploded_something = false;

			SnailPtr child = nullptr;
			if (s->m_left.index() == 1) 
			{
				child = std::move(std::get<1>(s->m_left));
				s->m_left = ExplodingPlaceHolder;
			}
			else if (s->m_right.index() == 1)
			{
				child = std::move(std::get<1>(s->m_right));
				s->m_right = ExplodingPlaceHolder;
			}


			if (child != nullptr)
			{
				//add the left digit to the nearest number to the left if there is one
				if (m_left_num != nullptr)
				{
					*m_left_num += std::get<0>(child->m_left);
					m_left_num = nullptr;
				}
				//store the right digit to add to the nearest number to the right if there is one
				m_right_num = std::get<0>(child->m_right);

				exploded_something = true;
			}

			return exploded_something;

		}

		enum class State
		{
			FindExplodingPair,    //in this state we update left num to point to the last num we see
			FindPlaceHolder,
			FindNextNumber,		  //in this state we are looking for a number to the right that we can add to
			Finished
		};

		State m_state = State::FindExplodingPair;
		Number* m_left_num = nullptr;
		Number  m_right_num = 0;
	
	};


	class Splitter : public Visitor
	{
	public:
		bool visit(Number& n, int depth, SnailPair* parent) override
		{
			bool carry_on = true;
		
			switch (m_state)
			{
			case State::FindingNumToSplit:
				if (n >= 10)
				{
					
					Number left = n / 2;
					Number right = (n + 1) / 2;
					auto new_pair = std::make_unique<SnailPair>(left,right, depth+1);  

					assert(parent != nullptr);
					parent->swap_in_new_pair(std::move(new_pair), n);

					carry_on = false;
					m_state = State::Finished;
				}
				break;

			case State::Finished:
				carry_on = false;
				break;
			}

			return carry_on;
		}

		bool visit(SnailPair* s, int depth, SnailPair* parent) override
		{
			bool carry_on = true;

			switch (m_state)
			{
			case State::FindingNumToSplit:			
				break;

			case State::Finished:
				carry_on = false;			
				break;
			}

			return carry_on;
		}

		bool did_split() const
		{
			return m_state != State::FindingNumToSplit;
		}

	private:
		enum class State
		{
			FindingNumToSplit,
			Finished
		};
		State m_state = State::FindingNumToSplit;
		
	};


	class Walker
	{
	public:
		Walker(Visitor& v, int depth, SnailPair* parent)
			: m_v{v}
			, m_depth{depth}
			, m_parent{ parent }
		{}

		bool operator()(Number& n)
		{
			return m_v.visit(n, m_depth, m_parent);			
		}

		bool operator()(SnailPtr& s)
		{
			return SnailPair::walk_tree(s.get(), m_v, m_depth + 1, m_parent);			
		}
	private:
		Visitor& m_v;
		int m_depth;
		SnailPair* m_parent;
	};
	
	static bool walk_tree(SnailPair* pair, Visitor& v, int depth, SnailPair* parent)
	{
		auto walker = Walker{v, depth, pair};
	
		
		if (!std::visit(walker, pair->m_left))
		{
			return false;
		}	
		
		if (!v.visit(pair, depth, parent))
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
		walk_tree(this, exploder, 1, nullptr);
		if (exploder.exploded())
		{
			//walk tree again and find first number after the placeholder and 
			walk_tree(this, exploder, 1, nullptr);
			return true;
		}
		else
		{
			return false;
		}
	}


	bool split_something()
	{
		Splitter splitter{};
		walk_tree(this, splitter, 1, nullptr);
		return splitter.did_split();		
	}

	void swap_in_new_pair(SnailPtr split, int val)
	{
		if (m_left.index() == 0 and std::get<0>(m_left) == val)
		{
			m_left = std::move(split);
		}
		else if (m_right.index() == 0 and std::get<0>(m_right) == val)
		{
			m_right = std::move(split);
		}
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
	
	Number cur_val = 0;
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
#if 0
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

#endif
#if 0
	std::vector<SnailPtr> snails{};

	snails.emplace_back(string_to_snail_pair("[1, 1]"));
	snails.emplace_back(string_to_snail_pair("[2, 2]"));
	snails.emplace_back(string_to_snail_pair("[3, 3]"));
	snails.emplace_back(string_to_snail_pair("[4, 4]"));
	snails.emplace_back(string_to_snail_pair("[5, 5]"));
	snails.emplace_back(string_to_snail_pair("[6, 6]"));
#endif
	auto snails = input::read_vector("../input_files/day18_example.txt", string_to_snail_pair);
	
	//accumulate
	SnailPtr bob = nullptr;
	for (auto& s : snails)
	{ 
		bob = add_snails(std::move(bob), std::move(s));
		//bob->print();
	}
	bob->print();
	//get magnitude
	return 0;
}  