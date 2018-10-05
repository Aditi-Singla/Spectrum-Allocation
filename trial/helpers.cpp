#include "tower_allocation.h"

std::map<int,bid> bid_map;
std::map<int,company> bid_company;

float collision_cost = INT_MAX; //TODO:some amount to tweak or infer

float prob_swap = 0.5;
float prob_add = 0.5;
float prob_delete = 0;

float prob_restart = 0.0;

float time_fact = 0.8;

float input_time = 0;
int no_of_bids = 0; // TODO: Check default initialization
int no_of_companies = 0; // TODO: Check default initialization
int no_of_regions = 0; // TODO: Check default initialization

/**
 * Function definition for bid class
 */

void bid::set_cost(float cost_in)
{
	cost = cost_in;
}

void bid::add_region(int reg_no)
{
	set_of_regions.push_back(reg_no);
}

void bid::add_company(int c_id)
{
	company = c_id;
}

/**
 * Function definition for company class
 */

 void company::add_bid(int bid_id)
 {
 	list_of_bids.push_back(bid_id);
 }

/**
 * Function definition for state class
 */ 

float state::get_cost()
{
	float state_cost = 0;

	std::map<int, std::vector<int> > map;

	for (std::vector<int>::iterator i = list_of_bids.begin(); i != list_of_bids.end(); ++i)
	{
		state_cost+=bid_map[*i].cost; // add bid selection cost

		// for (std::vector<int>::iterator j = bid_map[*i].set_of_regions.begin(); j != bid_map[*i].set_of_regions.end(); ++j)
		// {
		// 	map[*j].push_back(*i); // to determine total number of collisions
		// }
	}

	// for(std::map<int, std::vector<int> >::iterator i = map.begin(); i != map.end(); i++)
	// {
	// 	state_cost -= ((i -> second).size() - 1)*collision_cost; // subtracting cost of collision
	// }

	return state_cost;
}