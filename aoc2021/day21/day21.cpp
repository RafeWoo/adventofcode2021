
#include <array>
#include <iostream>
#include <format>

class Game
{
public:

	Game( int player_1_start, int player_2_start)
		: m_players{ PosAndScore{player_1_start}, PosAndScore{player_2_start} }
	{}


	bool is_finished() const
	{
		return (m_players[0].m_score >= 1000) or
			(m_players[1].m_score >= 1000);
	}

	int num_turns() const
	{
		return m_number_of_turns;
	}

	int losing_score() const
	{
		if (m_players[0].m_score >= 1000)
		{
			return m_players[1].m_score;
		}
		else
		{
			return m_players[0].m_score;
		}
	}

	void advance()
	{

		auto& player = m_players[m_next_player];

		auto next_pos = ((((player.m_pos + 3 + 3 * m_next_die_state) % 10) -10) %10) + 10;

		player.m_pos = next_pos;
		player.m_score += next_pos;
		m_next_die_state += 3;
		m_next_player = (m_next_player + 1) % 2;
		m_number_of_turns++;
	}

private:

	class PosAndScore
	{
	public:
		PosAndScore( int start_pos )
			: m_pos{start_pos}
		{}

		int m_pos;
		int m_score = 0;
	};
	std::array<PosAndScore,2> m_players;

	int m_next_player = 0;

	int m_next_die_state = 1;

	int m_number_of_turns = 0;
};


int main(void)
{

	Game game{ 1,5 };

	while (!game.is_finished())
	{
		game.advance();
	}

	int64_t num_turns = game.num_turns();
	int64_t losing_score = game.losing_score();

	auto result = 3* num_turns * losing_score;

	std::cout << std::format("The result is {}", result);
	return 0;
}