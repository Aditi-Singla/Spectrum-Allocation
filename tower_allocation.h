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

		void set_cost(float cost_in);
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
		std::map<int,bool> remaining_companies;

		state(state& s)
		{
			list_of_bids = s.list_of_bids;
			remaining_companies = s.remaining_companies;
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

std::map<int,bid> bid_map;
std::map<int,company> bid_company;

float collision_cost = 100; //TODO:some amount to tweak or infer

float prob_swap = 0.3;
float prob_add = 0.3;
float prob_delete = 0.3;

float prob_restart = 0.1;