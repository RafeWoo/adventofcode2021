module;
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>


export module Bingo;


export namespace Bingo
{

	class Board final
	{
	public:

		Board() = default;
		~Board() = default;

		Board(std::vector<int> const& board)
		{
			std::copy_n(board.begin(), x_dim * y_dim, m_board.begin() );
		}

		void apply_number(int number)
		{
			std::replace(std::begin(m_board)
				, std::end(m_board)
				, number
				, -1
			);
		}

		bool is_winner() const
		{
			//if any row or column is complete		
			for (int i = 0; i < x_dim; ++i)
			{
				if (is_col_complete(i))
				{
					return true;
				}
			}

			for (int i = 0; i < y_dim; ++i)
			{
				if (is_row_complete(i))
				{
					return true;
				}
			}

			return false;
		}

		int calc_score() const
		{
			int score = std::accumulate(std::begin(m_board), std::end(m_board), 0,
				[](auto const& a, auto const& b)
				{
					return a + (b >= 0 ? b : 0);
				});
			return score;
		}

	private:
		bool is_row_complete(int i) const
		{
			auto row_start = std::begin(m_board) + x_dim * i;
			auto row_end = row_start + x_dim;
			auto sum = std::accumulate(row_start, row_end, 0);
			return sum == -5;
		}

		bool is_col_complete(int col) const
		{
			int sum = 0;
			for (int row = 0; row < y_dim; ++row)
			{
				sum += m_board[row * x_dim + col];
			}
			return sum == -5;
		}

		static constexpr size_t x_dim = 5;
		static constexpr size_t y_dim = 5;
		std::array<int, x_dim* y_dim> m_board;
	};


	
}

