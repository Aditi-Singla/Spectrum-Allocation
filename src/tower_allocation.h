#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <limits.h>
#include <random>
#include <math.h>
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
			// std::cout << "new bid\n";
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
		std::vector<int>/*std::unordered_map<int,bool>*/ remaining_companies;

		state()
		{

		}

		state(const state &s)
		{
			list_of_bids = s.list_of_bids; // TODO: Check for shallow or deep copy
			remaining_companies = s.remaining_companies;
		}

		float get_cost();
		float get_actual_cost();
};

class allocate
{
	public:
		state best_state;
		state get_start_state();
		state get_random_state();
		void search(float time,state start_state);
		void greedy_search(float time,state start_state);
		void greedy_random_search(float time,state start_state);
		void sa_search(float time,state start_state);
		state remove_collision(state input_state);
		void input(char filename[]);
		void output(state output_state, char filename[]);

		// allocate()
		// {
		// 	best_state = NULL;	
		// }
};

float random_fraction();
state get_random_neighbour(state temp);

extern std::unordered_map<int,bid> bid_map;
extern std::unordered_map<int,company> bid_company;

extern float collision_cost; //TODO:some amount to tweak or infer

extern float prob_swap;
extern float prob_add;
extern float prob_delete;

extern float prob_restart;

extern float time_fact;

extern float input_time;
extern int no_of_bids; // TODO: Check default initialization
extern int no_of_companies; // TODO: Check default initialization
extern int no_of_regions; // TODO: Check default initialization
extern std::unordered_map<int,bid>::iterator mit;
extern std::vector<int> bid_ids;
extern std::vector<int>::iterator bid_it;