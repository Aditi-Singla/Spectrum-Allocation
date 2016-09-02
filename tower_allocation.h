#include <iostream>
#include <vector>
#include <map>
#include <fstream> // TODO: Check if necessary
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#define max 10000

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
		std::vector<int>/*std::map<int,bool>*/ remaining_companies;

		state()
		{

		}

		state(state& s)
		{
			list_of_bids = s.list_of_bids; // TODO: Check for shallow or deep copy
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
		state remove_collision(state input_state);
		void input();
		void output(state output_state);

		// allocate()
		// {
		// 	best_state = NULL;	
		// }
};

float random_fraction();

std::map<int,bid> bid_map;
std::map<int,company> bid_company;

float collision_cost = 100; //TODO:some amount to tweak or infer

float prob_swap = 0.3;
float prob_add = 0.3;
float prob_delete = 0.3;

float prob_restart = 0.1;

float time_fact = 0.8;

float input_time = 0;
int no_of_bids = 0; // TODO: Check default initialization
int no_of_companies = 0; // TODO: Check default initialization
int no_of_regions = 0; // TODO: Check default initialization