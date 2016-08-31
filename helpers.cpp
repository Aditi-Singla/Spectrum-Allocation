#include "tower_allocation.h"

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

		for (std::vector<int>::iterator j = bid_map[*i].set_of_regions.begin(); j != bid_map[*i].set_of_regions.end(); ++j)
		{
			map[*j].push_back(*i); // to determine total number of collisions
		}
	}

	for(std::map<int, std::vector<int> >::iterator i = map.begin(); i != map.end(); i++)
	{
		state_cost += ((i -> second).size() - 1)*collision_cost; // cost of collision
	}

	return state_cost;
}