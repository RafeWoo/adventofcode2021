
#include <iostream>
#include <format>
#include <vector>
#include <tuple>
#include <set>

int triangular_number(int n)
{
	return (n * (n + 1)) / 2;
}
//calc pos given initial velocity and time step
int ypos(int vel, int timestep)
{
	return (vel * timestep) - triangular_number(timestep - 1);
}

int xpos(int vel, int timestep)
{
	auto xp1 = triangular_number(vel);
	auto xp2 = (vel > timestep) ? triangular_number(vel - timestep) : 0;
	auto xp3 = xp1 - xp2;
	return  xp3;
}

int main(void)
{
	//we want biggest yvel such that ypos at some point is between -121 and -159 inclusive
	//then we want to find the highest point reached.

#if 0
	int yvel = 0;

	while (yvel < 300)
	{
		int step = 1;


		auto yp = ypos(yvel, step);
		while (yp >= -159)
		{
			if (-159 <= yp and yp <= -121)
			{
				std::cout << std::format("yvel={},", yvel);
				std::cout << std::format(" step={}, pos={} \n ", step, yp);
			}
			step++;
			yp = ypos(yvel, step);
		}
		std::cout << "\n";

		++yvel;
	}


	auto yvel = 158;
	int64_t max_height = 0;
	int max_step = 0;
	for (int step = 1; step < 318; ++step)
	{
		auto yp = ypos(yvel, step);
		if (yp > max_height)
		{
			max_height = yp;
			max_step = step;
		}
	}

#endif

	//part 2
#if 0

	std::set<std::pair<int, int> > example_vels =
	{
		{23,-10}, {25,-9},   {27,-5},   {29,-6}, {22,-6}, {21,-7},   {9,0},     {27,-7},   {24,-5},
{25,-7   },{26,-6  },{ 25,-5  },{ 6,8    },{ 11,-2 },{  20,-5 },{  29,-10},{  6,3   },{  28,-7},
{8,0     },{30,-6  },{ 29,-8  },{ 20,-10 },{ 6,7   },{  6,4   },{  6,1   },{  14,-4 },{  21,-6},
{26,-10  },{7,-1   },{ 7,7    },{ 8,-1   },{ 21,-9 },{  6,2   },{  20,-7 },{  30,-10},{  14,-3},
{20,-8   },{13,-2  },{ 7,3    },{ 28,-8  },{ 29,-9 },{  15,-3 },{  22,-5 },{  26,-8 },{  25,-8},
{25,-6   },{15,-4  },{ 9,-2   },{ 15,-2  },{ 12,-2 },{  28,-9 },{  12,-3 },{  24,-6 },{  23,-7},
{25,-10  },{7,8    },{ 11,-3  },{ 26,-7  },{ 7,1   },{  23,-9 },{  6,0   },{  22,-10},{  27,-6},
{8,1     },{22,-8  },{ 13,-4  },{ 7,6    },{ 28,-6 },{  11,-4 },{  12,-4 },{  26,-9 },{  7,4  },
{24,-10  },{23,-8  },{ 30,-8  },{ 7,0    },{ 9,-1  },{  10,-1 },{  26,-5 },{  22,-9 },{  6,5  },
{7,5     },{23,-6  },{ 28,-10 },{ 10,-2  },{ 11,-1 },{  20,-9 },{  14,-2 },{  29,-7 },{  13,-3},
{23,-5   },{24,-8  },{ 27,-9  },{ 30,-7  },{ 28,-5 },{  21,-10},{  7,9   },{  6,6   },{  21,-5},
{27,-10  },{7,2    },{ 30,-9  },{ 21,-8  },{ 22,-7 },{  24,-9 },{  20,-6 },{  6,9   },{  29,-5},
{8,-2    },{27,-8  },{ 30,-5  },{ 24,-7	 }
	};




	constexpr int X_MIN = 20;
	constexpr int X_MAX = 30;
	constexpr int Y_MAX = -5;
	constexpr int Y_MIN = -10;
#else
	constexpr int X_MIN = 70;
	constexpr int X_MAX = 125;
	constexpr int Y_MAX = -121;
	constexpr int Y_MIN = -159;
#endif

	// find all pairs {xv, yv} such that at somepoint n {xpos_n, ypos_n} is in  x=70,125    y=-121, -159 inclusive

	//let xvel range from 12 to 30
	//let yvel range from -159 to 158
	//let t range from 1 to 158

	std::set< std::pair<int, int> > valid_velocities{};


	for (int xvel = 12; xvel <= 125; ++xvel) //12 is the smallest xvel since wont make target otherwise, 126 will take us past
	{
		for (int yvel = -160; yvel <= 158; ++yvel)  //lower than -160 will take us past the target range. >158 is our top vel from part 1
		{
			for (int t = 1; t <= 1000; ++t)  
			{

				int xp = xpos(xvel, t);
				auto yp = ypos(yvel, t);

				if ( X_MIN <= xp and xp <= X_MAX					
					and Y_MIN <= yp and yp <= Y_MAX) 
				{
						valid_velocities.emplace(xvel, yvel);
						break;
				}			
				else if (xp > X_MAX or yp < Y_MIN) //gone past
				{
					break;
				}

			}
		}

	}
	
	std::cout << std::format("There are {} vels that end in target zone", valid_velocities.size());
	return 0;
}