#include "tower_allocation.h"

float random_fraction()
{
	std::srand(std::time(0)); // use current time as seed for random generator
    int random_variable = std::rand();
    float ans = random_variable/RAND_MAX;
    return ans;
}

state allocate::get_start_state()
{
	state new_state;

	int loop_max = (int)(random_fraction()*(no_of_companies -1));

	std::map<int, bool> map; // to check whether a company has occurred

	for (int i = 0; i <= loop_max; ++i) // to fix a random no. of companies
	{
		int num = (int)(random_fraction()*(no_of_companies -1));

		while(map.find(num) != map.end())
		{
			num = (int)(random_fraction()*(no_of_companies -1));
		}

		map[num] = false;

		int bid_num = (int)(random_fraction()*(bid_company[num].list_of_bids.size()-1));

		new_state.list_of_bids.push_back(bid_company[num].list_of_bids[bid_num]);
	}

	for (int i = 0; i < no_of_companies; ++i)
	{
		if(map.find(i) == map.end())
			new_state.remaining_companies.push_back(i);
	}

	if (best_state.list_of_bids.size() == 0)
		best_state = new_state;
	else if (best_state.get_cost() <= new_state.get_cost()) // TODO: Check for equality here
		best_state = new_state;

	return new_state;
}

void allocate::search(float inp_time, state start_state)
{
	state current = start_state; //initially

	time_t start = time(0);

	// double seconds_since_start = difftime( time(0), start);

	while(difftime( time(0), start) <= time_fact * inp_time) // TODO: Check for time elapsed
	{
		float fract = random_fraction();

		if (fract <= prob_swap)
		{
			// code for swap
			// for the time being the swap is from an open company to itself
			state newstate = current;
			bool done = false;

			for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() && difftime( time(0), start) <= time_fact * inp_time; ++i)
			{
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

				int comp_id = bid_map[*i].company;

				for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end() && difftime( time(0), start) <= time_fact * inp_time; ++j)
				{
					if(*i != *j)
					{
						newstate.list_of_bids.push_back(*j);
						if (newstate.get_cost() >= current.get_cost())
						{
							current = newstate;
							done = true;
							break;
						}
						else
						{
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
						}
					}
				}

				if(done)
				{
					break;
				}
				else
				{
					newstate.list_of_bids.push_back(*i);
				}
			}

			if(done)
			{
				if(best_state.get_cost() <= current.get_cost())
					best_state = current;
			}
			// else
			// {
			// 	current = get_start_state(); // restart
			// }

		}
		else if (fract <= prob_swap+prob_add && fract > prob_swap)
		{
			// code for add

			state newstate = current;
			bool done = false;

			for (std::vector<int>::iterator i = current.remaining_companies.begin(); i != current.remaining_companies.end() && difftime( time(0), start) <= time_fact * inp_time; ++i)
			{
				// newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), *i), newstate.list_of_bids.end());	

				int comp_id = *i;

				for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end() && difftime( time(0), start) <= time_fact * inp_time; ++j)
				{
					// if(*i != *j)
					// {
						newstate.list_of_bids.push_back(*j);
						if (newstate.get_cost() >= current.get_cost())
						{
							newstate.remaining_companies.erase(std::remove(newstate.remaining_companies.begin(), newstate.remaining_companies.end(), comp_id), newstate.list_of_bids.end());	
							current = newstate;
							done = true;
							break;
						}
						else
						{
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
						}
					// }
				}

				if(done)
				{
					break;
				}
			}

			if(done)
			{
				if(best_state.get_cost() <= current.get_cost())
					best_state = current;
			}
			// else
			// {
			// 	current = get_start_state(); // restart
			// }
		}
		else if (fract <= prob_swap+prob_add+prob_delete > prob_swap+prob_add)
		{
			// code for delete

			state newstate = current;
			bool done = false;

			for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() && difftime( time(0), start) <= time_fact * inp_time; ++i)
			{
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

				int comp_id = bid_map[*i].company;

				if (newstate.get_cost() >= current.get_cost())
				{
					newstate.remaining_companies.push_back(comp_id);
					current = newstate;
					done = true;
					break;
				}
				else
				{
					newstate.list_of_bids.push_back(*i);
				}
			}

			if(done)
			{
				if(best_state.get_cost() <= current.get_cost())
					best_state = current;
			}
			// else
			// {
			// 	current = get_start_state(); // restart
			// }
		}
		else
		{
			// random restart
			current = get_start_state();	
		}
	}
}