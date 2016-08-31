#include <iostream>
#include <vector>
#include <map>

class bid
{
	public:
		float cost;
		std::vector<int> set_of_regions;
		int company;

		bid()
		{
			cost = 0;
			company = -1;
		}

		void set_cost(float cost);
		void add_region(int reg_no);
		void add_company(int c_id);
};

class company
{
	public:
		std::vector<int> list_of_bids;

		void add_bid(int bid_id);
};

class state
{
	public:
		std::vector<int> list_of_bids;
		std::map<int,bool> map;

		state(state& s)
		{
			list_of_bids = s.list_of_bids;
			map = s.map;
		}

		float get_cost();
};

class allocate
{
	public:
		state best_state;
		state get_start_state();
		void search(float time,state start_state);
		void remove_collision(state input_state);
		void input();
		void output(state output_state);
};