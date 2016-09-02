#include <string>
#include <sstream>
#include <cstdio>
#include "tower_allocation.h"
using namespace std;

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

	while(difftime(time(0), start) <= time_fact * inp_time) // TODO: Check for time elapsed
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
		else if (fract <= prob_swap+prob_add+prob_delete && fract > prob_swap+prob_add)
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

state allocate::remove_collision(state input_state)
{
	std::map<int, std::vector<int> > map; // region,list of bids

	for (std::vector<int>::iterator i = input_state.list_of_bids.begin(); i != input_state.list_of_bids.end(); ++i)
	{
		for (std::vector<int>::iterator j = bid_map[*i].set_of_regions.begin(); j != bid_map[*i].set_of_regions.end(); ++j)
		{
			map[*j].push_back(*i); // to determine total number of collisions
		}
	}

	for (std::map<int, std::vector<int> >::iterator i = map.begin(); i != map.end(); ++i)
	{
		int min_collision = 0;
		int min_served = 0;
		int min_bid_id = 0;

		if ((i -> second).size() > 1) // if 2 or more regions collide
		{
			for (std::vector<int>::iterator j = map[i->first].begin(); j != map[i->first].end(); ++j)
			{
				int count = 0; int served = 0;

				for (std::vector<int>::iterator k = bid_map[*j].set_of_regions.begin(); k != bid_map[*j].set_of_regions.end(); ++k)
				{
					if (map[*k].size() > 1)
					{
						count++;
					}
					else if(map[*k].size() == 1)
					{
						served++;
					}
				}

				if (count < min_collision)
				{
					min_bid_id = *j;
					min_collision = count;
					min_served = served;
				}
				else if (count == min_collision)
				{
					if (served < min_served)
					{
						min_bid_id = *j;
						min_collision = count;
						min_served = served;
					}
				}

			}
		}

		map[i -> first].clear();
		map[i -> first].push_back(min_bid_id);
	}

	state final_state;

	std::map<int, bool> comp_incl;

	for (std::map<int, std::vector<int> >::iterator i = map.begin(); i != map.end(); ++i)
	{
		if (map[i -> first].size() != 0)
		{
			int temp = map[i -> first].back(); // getting the bid id

			std::vector<int>::iterator it;

			it = find (final_state.list_of_bids.begin(), final_state.list_of_bids.end(), temp);
			
			if (it == final_state.list_of_bids.end())
				final_state.list_of_bids.push_back(temp);

			comp_incl[bid_map[temp].company] = true;
		}
	}

	for (std::map<int, company >::iterator i = bid_company.begin(); i != bid_company.end(); ++i)
	{
		if (comp_incl.find(i -> first) == comp_incl.end())
		{
			// company not included in bidding
			// we will try and include some bids
			float max_cost = 0;
			int max_bid_id = 0;
			bool found = false;

			for (std::vector<int>::iterator j = bid_company[i -> first].list_of_bids.begin(); j != bid_company[i -> first].list_of_bids.end(); ++j)
			{
				bool flag = true;

				for (std::vector<int>::iterator ii = bid_map[*j].set_of_regions.begin(); ii != bid_map[*j].set_of_regions.end(); ++ii)
				{
					if (map[*j].size() != 0)
					{
						flag = false;
					}	
				}

				if(flag)
				{
					if (bid_map[*j].cost > max_cost)
					{
						max_cost = bid_map[*j].cost;
						max_bid_id = *j;
						found = true;
					}
				}
			}

			if(found)
				final_state.list_of_bids.push_back(max_bid_id); // select the bid
		}
	}
	return final_state;
}

std::vector<string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;
	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

void allocate::input()
{
	string temp;
	int no_of_regions, no_of_bids, no_of_companies;
	int k = 0;
	ifstream in;
	in.open("1.txt");
	if (!in.is_open()){
		cout << "File not found.\n";
	}
	else{
		in >> temp;
		input_time = stoi(temp);
		in >> temp;
		no_of_regions = stoi(temp);
		in >> temp;
		no_of_bids = stoi(temp);
		in >> temp;
		no_of_companies = stoi(temp);
		for (int i=0; i<no_of_bids; i++){
			if (k >= no_of_companies){
				cout << "Invalid Input. \n";
				return;
			}
			else{
				in >> temp;
				vector<string> l = split(temp, ' ');
				bid *b;
				int c_id = stoi(l.at(0));
				b->add_company(c_id);
				b->set_cost(stof(l.at(1)));
				for (int j=2; j<l.size()-1; j++){
					b->add_region(stof(l.at(j)));
				}
				bid_map.insert(std::pair<int,bid>(i,*b));
				if (bid_company.find(c_id) == bid_company.end()){
					company *c;
					c->add_bid(i);
					bid_company.insert(std::pair<int,company>(c_id,*c));
					delete c;
				}
				delete b;
			}
		}
	}
}

void allocate::output(state output_state)
{
	ofstream out("out.txt", ios::app);
	vector<int> list_of_bids = output_state.list_of_bids;
	for (int i=0; i<list_of_bids.size(); i++){
		out << list_of_bids.at(i) << " ";
	}
	out.close();
}