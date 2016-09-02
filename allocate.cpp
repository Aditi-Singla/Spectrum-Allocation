#include <string>
#include <sstream>
#include <cstdio>
#include "tower_allocation.h"
using namespace std;

float random_fraction()
{
	// std::srand(std::time(0)); // use current time as seed for random generator
    int random_variable = std::rand();
    float ans = (float)random_variable/(float)RAND_MAX;

    std::cout << "random::"<<ans<<"\n";
    return ans;

//------------------------------------------------------------
	// std::default_random_engine generator;
 //  	std::uniform_int_distribution<int> distribution(0,INT_MAX);

 //  	int num = distribution(generator);

 //  	return (float)num/(float)INT_MAX;

	// return (float)(random())/(float)(INT_MAX);
}

void output_state_details(state s)
{
	std::cout << "Cost: " << s.get_cost() << "\n";

	for (std::vector<int>::iterator i = s.list_of_bids.begin(); i != s.list_of_bids.end(); ++i)
	{
		std::cout<<*i<<" ";
	}

	std::cout << std::endl;
}

bool check_collision(std::vector<int> v,int new_bid)
{
	std::map<int,bool> map;

	if (new_bid >= 0)
	for (std::vector<int>::iterator i = bid_map[new_bid].set_of_regions.begin(); i != bid_map[new_bid].set_of_regions.end(); ++i)
	{
		map[*i] = true;
	}

	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i)
	{
		for (std::vector<int>::iterator j = bid_map[*i].set_of_regions.begin(); j != bid_map[*i].set_of_regions.end(); ++j)
		{
			if(map.find(*j) != map.end())
				return false;
			else
				map[*j] = true;
		}
	}

	return true;

}

std::map<int,bool> start_state_occur;


state allocate::get_start_state()
{
	state new_state;

	// std::srand(0);
	// do
	// {
	// 	new_state.list_of_bids.clear();
	// 	new_state.remaining_companies.clear();

		// int loop_max = (int)(random_fraction()*((float)(no_of_companies -1)));
		int loop_max = 0;
		std::cout<<"noc::"<<no_of_companies-1<<"\n";
		std::cout<<"loopmax::"<<loop_max<<"\n";

		std::map<int, bool> map; // to check whether a company has occurred

		for (int i = 0; i <= loop_max; ++i) // to fix a random no. of companies
		{
			int num = (int)(random_fraction()*(no_of_companies -1));
			std::cout<<"num:"<<num<<"\n";
			while(map.find(num) != map.end())
			{
				num = (int)(random_fraction()*(no_of_companies -1));
			}
			std::cout<<"yo\n";
			map[num] = false;

			//-----------

			// int bid_num = ::mit -> first;
			// std::cout<<"here";
			// mit++;

			//-----------

			int bid_num = (int)(random_fraction()*(bid_company[num].list_of_bids.size()-1));
			std::cout<<"bidnum:"<<bid_num<<" "<<bid_company[num].list_of_bids.size()<<"\n";
			new_state.list_of_bids.push_back(bid_company[num].list_of_bids[bid_num]);

			// // new_state.list_of_bids.push_back(bid_num);
			// map[bid_map[bid_num].company] = false;

		}

		for (int i = 0; i < no_of_companies; ++i)
		{
			if(map.find(i) == map.end())
				new_state.remaining_companies.push_back(i);
		}

	// 	if(start_state_occur.find(new_state.list_of_bids[0]) == start_state_occur.end())
	// 	{
	// 		start_state_occur[new_state.list_of_bids[0]] = true;
	// 		break;	
	// 	} 

	// }
	// while(/*new_state.get_cost() <= 0*//*!check_collision(new_state.list_of_bids,-1)*/ true);

	if (best_state.list_of_bids.size() == 0)
		best_state = new_state;
	else if (best_state.get_cost() <= new_state.get_cost()) // TODO: Check for equality here
		best_state = new_state;

	return new_state;
}

void allocate::greedy_search(float inp_time, state start_state)
{
	state current = start_state; //initially

	output_state_details(start_state);

	time_t start = time(0);

	// double seconds_since_start = difftime( time(0), start);

	while(difftime(time(0), start) <= time_fact * inp_time*60 /*true*/ ) // TODO: Check for time elapsed
	{
		std::cout<<"diff: "<<difftime(time(0), start)<<"\n";
		float fract = random_fraction();

		// if (fract <= prob_swap)
		// {
			// code for swap
			// for the time being the swap is from an open company to itself
			std::cout<<"SWAP:\n";
			state newstate = current;
			float max_assign_cost = current.get_cost();
			state max_assign_state = current;
			bool done = false;

			for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
			{
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

				int comp_id = bid_map[*i].company;
				
				for (std::map<int,bid>::iterator j = bid_map.begin(); j != bid_map.end() && (((j->second).company == comp_id) || (find(current.remaining_companies.begin(),current.remaining_companies.end(),(j -> second).company) != current.remaining_companies.end())) /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++j)
				{
					std::vector<int>::iterator it = find(current.remaining_companies.begin(),current.remaining_companies.end(),(j -> second).company);
					if((*i != (j->first)) && (((j -> second).company == comp_id) || it != current.remaining_companies.end() ))
					{
						newstate.list_of_bids.push_back((j->first));
						if (newstate.get_cost() >= max_assign_cost)
						{
							max_assign_state = newstate;
							max_assign_state.remaining_companies.erase(std::remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (bid_map[(j->first)].company)), max_assign_state.remaining_companies.end());	
							// max_assign_state.remaining_companies.push_back(bid_map[*i].company);
							max_assign_cost = max_assign_state.get_cost();
							done = true;
							// break;
						}
						// else
						// {
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((j->first))), newstate.list_of_bids.end());	
						// }
					}
				}

				// if(done)
				// {
				// 	break;
				// }
				// else
				// {
					newstate.list_of_bids.push_back(*i);
				// }
			}

			for (std::vector<int>::iterator i = current.remaining_companies.begin(); i != current.remaining_companies.end(); ++i)
			{
				for (std::vector<int>::iterator j = bid_company[*i].list_of_bids.begin(); j != bid_company[*i].list_of_bids.end(); ++j)
				{
						newstate.list_of_bids.push_back((*j));
						if (newstate.get_cost() >= max_assign_cost)
						{
							max_assign_state = newstate;
							max_assign_state.remaining_companies.erase(std::remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), bid_map[*j].company), max_assign_state.remaining_companies.end());	
							// max_assign_state.remaining_companies.erase(std::remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (bid_map[(*i)].company)), max_assign_state.remaining_companies.end());	
							max_assign_cost = max_assign_state.get_cost();
							done = true;
							// break;
						}
						// else
						// {
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((*j))), newstate.list_of_bids.end());	
				}
			}
			

			// if(done)
			// {
				if(best_state.get_cost() <= max_assign_state.get_cost())
					best_state = max_assign_state;

				current = max_assign_state;
			// }
			// else
			// {
			// 	current = get_start_state(); // restart
			// }

		// }
		// else if (fract <= prob_swap+prob_add && fract > prob_swap)
		// {
			// code for add
			// std::cout<<"ADD:\n";
			// state newstate = current;
			// bool done = false;
			// std::cout<<"done1\n";
		// 	for (std::vector<int>::iterator i = current.remaining_companies.begin(); i != current.remaining_companies.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
		// 	{
		// 		// newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), *i), newstate.list_of_bids.end());	

		// 		int comp_id = *i;
		// 		std::cout<<"done2\n";
		// 		for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++j)
		// 		{
		// 			// if(*i != *j)
		// 			// {
		// 				newstate.list_of_bids.push_back(*j);
		// 				if (newstate.get_cost() >= current.get_cost())
		// 				{
		// 					std::cout<<newstate.remaining_companies.size()<<"\n";
		// 					newstate.remaining_companies.erase(std::remove(newstate.remaining_companies.begin(), newstate.remaining_companies.end(), comp_id), newstate.remaining_companies.end());	
		// 					std::cout<<"done3\n";
		// 					current = newstate;
		// 					done = true;
		// 					break;
		// 				}
		// 				else
		// 				{
		// 					newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
		// 					std::cout<<"done4\n";
		// 				}
		// 			// }
		// 		}

		// 		if(done)
		// 		{
		// 			break;
		// 		}
		// 	}

		// 	if(done)
		// 	{
		// 		if(best_state.get_cost() <= current.get_cost())
		// 			best_state = current;
		// 	}
		// 	// else
		// 	// {
		// 	// 	current = get_start_state(); // restart
		// 	// }
		// }
		// else if (fract <= prob_swap+prob_add+prob_delete && fract > prob_swap+prob_add)
		// {
		// 	// code for delete
		// 	std::cout<<"DELETE:\n";
		// 	state newstate = current;
		// 	bool done = false;

		// 	for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
		// 	{
		// 		newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

		// 		int comp_id = bid_map[*i].company;

		// 		if (newstate.get_cost() >= current.get_cost())
		// 		{
		// 			newstate.remaining_companies.push_back(comp_id);
		// 			current = newstate;
		// 			done = true;
		// 			break;
		// 		}
		// 		else
		// 		{
		// 			newstate.list_of_bids.push_back(*i);
		// 		}
		// 	}

			if(!done)
			{
				// if(best_state.get_cost() <= current.get_cost())
				// 	best_state = current;
				current = get_start_state();	
			}
			// else
			// {
			// 	current = get_start_state(); // restart
			// }
		// }
		// else
		// {
		// 	// random restart
		// 	current = get_start_state();	
		// }

		std::cout<<"best\n";
		output_state_details(best_state);
		std::cout<<"current\n";
		output_state_details(current);
	}
}

void allocate::greedy_random_search(float inp_time, state start_state)
{
	state current = start_state; //initially

	output_state_details(start_state);

	time_t start = time(0);

	// double seconds_since_start = difftime( time(0), start);

	while(difftime(time(0), start) <= time_fact * inp_time*60 /*true*/ ) // TODO: Check for time elapsed
	{
		// std::srand(std::time(NULL));

		std::cout<<"diff: "<<difftime(time(0), start)<<"\n";
		float fract = random_fraction();

		state newstate = current;
		float max_assign_cost = current.get_cost();
		state max_assign_state = current;
		bool done = false;

		if (fract <= prob_swap+prob_add)
		{
			// code for swap
			// for the time being the swap is from an open company to itself
			std::cout<<"SWAP:\n";
			

			for (std::vector<int>::iterator i = current.remaining_companies.begin(); i != current.remaining_companies.end(); ++i)
			{
				for (std::vector<int>::iterator j = bid_company[*i].list_of_bids.begin(); (j != bid_company[*i].list_of_bids.end()) && (check_collision(newstate.list_of_bids,*j)); ++j)
				{
						newstate.list_of_bids.push_back((*j));
						if (newstate.get_cost() >= max_assign_cost)
						{
							max_assign_state = newstate;
							max_assign_state.remaining_companies.erase(std::remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), bid_map[*j].company), max_assign_state.remaining_companies.end());	
							// max_assign_state.remaining_companies.erase(std::remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (bid_map[(*i)].company)), max_assign_state.remaining_companies.end());	
							max_assign_cost = max_assign_state.get_cost();
							done = true;
							// break;
						}
						// else
						// {
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((*j))), newstate.list_of_bids.end());	
				}
			}

			for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
			{
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

				int comp_id = bid_map[*i].company;
				
				for (std::map<int,bid>::iterator j = bid_map.begin(); (j != bid_map.end()) && (check_collision(newstate.list_of_bids,j->first)) && (((j->second).company == comp_id) || (find(current.remaining_companies.begin(),current.remaining_companies.end(),(j -> second).company) != current.remaining_companies.end())) /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++j)
				{
					std::vector<int>::iterator it = find(current.remaining_companies.begin(),current.remaining_companies.end(),(j -> second).company);
					if((*i != (j->first)) && (((j -> second).company == comp_id) || it != current.remaining_companies.end() ))
					{
						newstate.list_of_bids.push_back((j->first));
						if (newstate.get_cost() >= max_assign_cost)
						{
							max_assign_state = newstate;
							max_assign_state.remaining_companies.erase(std::remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (bid_map[(j->first)].company)), max_assign_state.remaining_companies.end());	
							// max_assign_state.remaining_companies.push_back(bid_map[*i].company);
							max_assign_cost = max_assign_state.get_cost();
							done = true;
							// break;
						}
						// else
						// {
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((j->first))), newstate.list_of_bids.end());	
						// }
					}
				}

				// if(done)
				// {
				// 	break;
				// }
				// else
				// {
					newstate.list_of_bids.push_back(*i);
				// }
			}

		// 	if(best_state.get_cost() <= max_assign_state.get_cost())
		// 			best_state = max_assign_state;

		// 		current = max_assign_state;
		// }
		// else if((fract <= prob_swap+prob_add && fract > prob_swap))
		// {

			if(best_state.get_cost() <= max_assign_state.get_cost())
					best_state = max_assign_state;

				current = max_assign_state;

			if(!done)
			{
				// if(best_state.get_cost() <= current.get_cost())
				// 	best_state = current;
				// std::srand(std::time(NULL));
				current = get_start_state();	
			}
		}
		else
		{
			current = get_start_state();
		}
			

			// if(done)
			// {


				// if(best_state.get_cost() <= max_assign_state.get_cost())
				// 	best_state = max_assign_state;

				// current = max_assign_state;


			// }
			// else
			// {
			// 	current = get_start_state(); // restart
			// }

		// }
		// else if (fract <= prob_swap+prob_add && fract > prob_swap)
		// {
			// code for add
			// std::cout<<"ADD:\n";
			// state newstate = current;
			// bool done = false;
			// std::cout<<"done1\n";
		// 	for (std::vector<int>::iterator i = current.remaining_companies.begin(); i != current.remaining_companies.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
		// 	{
		// 		// newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), *i), newstate.list_of_bids.end());	

		// 		int comp_id = *i;
		// 		std::cout<<"done2\n";
		// 		for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++j)
		// 		{
		// 			// if(*i != *j)
		// 			// {
		// 				newstate.list_of_bids.push_back(*j);
		// 				if (newstate.get_cost() >= current.get_cost())
		// 				{
		// 					std::cout<<newstate.remaining_companies.size()<<"\n";
		// 					newstate.remaining_companies.erase(std::remove(newstate.remaining_companies.begin(), newstate.remaining_companies.end(), comp_id), newstate.remaining_companies.end());	
		// 					std::cout<<"done3\n";
		// 					current = newstate;
		// 					done = true;
		// 					break;
		// 				}
		// 				else
		// 				{
		// 					newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
		// 					std::cout<<"done4\n";
		// 				}
		// 			// }
		// 		}

		// 		if(done)
		// 		{
		// 			break;
		// 		}
		// 	}

		// 	if(done)
		// 	{
		// 		if(best_state.get_cost() <= current.get_cost())
		// 			best_state = current;
		// 	}
		// 	// else
		// 	// {
		// 	// 	current = get_start_state(); // restart
		// 	// }
		// }
		// else if (fract <= prob_swap+prob_add+prob_delete && fract > prob_swap+prob_add)
		// {
		// 	// code for delete
		// 	std::cout<<"DELETE:\n";
		// 	state newstate = current;
		// 	bool done = false;

		// 	for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
		// 	{
		// 		newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

		// 		int comp_id = bid_map[*i].company;

		// 		if (newstate.get_cost() >= current.get_cost())
		// 		{
		// 			newstate.remaining_companies.push_back(comp_id);
		// 			current = newstate;
		// 			done = true;
		// 			break;
		// 		}
		// 		else
		// 		{
		// 			newstate.list_of_bids.push_back(*i);
		// 		}
		// 	}

			// if(!done)
			// {
			// 	// if(best_state.get_cost() <= current.get_cost())
			// 	// 	best_state = current;
			// 	current = get_start_state();	
			// }


			// else
			// {
			// 	current = get_start_state(); // restart
			// }
		// }
		// else
		// {
		// 	// random restart
		// 	current = get_start_state();	
		// }

		std::cout<<"best\n";
		output_state_details(best_state);
		std::cout<<"current\n";
		output_state_details(current);
	}
}

void allocate::search(float inp_time, state start_state)
{
	state current = start_state; //initially

	output_state_details(start_state);

	time_t start = time(0);

	// double seconds_since_start = difftime( time(0), start);
	std::cout << "inp_time:" <<inp_time << std::endl;
	while(difftime(time(0), start) <= time_fact * inp_time*60 /*true*/ ) // TODO: Check for time elapsed
	{
		std::cout<<"diff: "<<difftime(time(0), start)<<"\n";
		float fract = random_fraction();

		if (fract <= prob_swap)
		{
			// code for swap
			// for the time being the swap is from an open company to itself
			std::cout<<"SWAP:\n";
			state newstate = current;
			bool done = false;

			for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() && difftime( time(0), start) <= time_fact * inp_time; ++i)
			{
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	

				int comp_id = bid_map[*i].company;

				for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++j)
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
			std::cout<<"ADD:\n";
			state newstate = current;
			bool done = false;
			std::cout<<"done1\n";
			for (std::vector<int>::iterator i = current.remaining_companies.begin(); i != current.remaining_companies.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
			{
				// newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), *i), newstate.list_of_bids.end());	

				int comp_id = *i;
				std::cout<<"done2\n";
				for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++j)
				{
					// if(*i != *j)
					// {
						newstate.list_of_bids.push_back(*j);
						if (newstate.get_cost() >= current.get_cost())
						{
							std::cout<<newstate.remaining_companies.size()<<"\n";
							newstate.remaining_companies.erase(std::remove(newstate.remaining_companies.begin(), newstate.remaining_companies.end(), comp_id), newstate.remaining_companies.end());	
							std::cout<<"done3\n";
							current = newstate;
							done = true;
							break;
						}
						else
						{
							newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
							std::cout<<"done4\n";
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
			std::cout<<"DELETE:\n";
			state newstate = current;
			bool done = false;

			for (std::vector<int>::iterator i = current.list_of_bids.begin(); i != current.list_of_bids.end() /*&& difftime( time(0), start) <= time_fact * inp_time*/; ++i)
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

		std::cout<<"best\n";
		output_state_details(best_state);
		std::cout<<"current\n";
		output_state_details(current);
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
	output_state_details(input_state);
	output_state_details(final_state);
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
	string temp,temp1,temp2,temp3,line;
	int no_of_regions, no_of_bids, no_of_companies;
	int k = 0;
	ifstream in;
	in.open("../A1/A1/benchmarks/8.txt");

	std::cout << "file opened\n";

	if (!in.is_open()){
		cout << "File not found.\n";
	}
	else{
		in >> temp;
		int t=0;int j=0;
		char ch11[max];

		std::cout << "here1\n";

		while (temp[t]!='\0')
		{
			ch11[j]=temp[t];
			j++;t++;
		}
		ch11[j]='\0';
		::input_time = (strtod (ch11, NULL));
		// input_time = (strtod (ch11, NULL));
		std::cout << "here2\n";
		
		in >> temp1;
		t = 0;
		j = 0;
		char ch12[max];
		while(temp1[t]!='\0')
		{
			ch12[j]=temp1[t];
			j++;t++;
		}
		ch12[j]='\0';
		::no_of_regions = atoi(ch12);
		no_of_regions = atoi(ch12);
		
		in >> temp2;
		t = 0;
		j = 0;

		std::cout << "here3\n";

		char ch13[max];
		while(temp2[t]!='\0')
		{
			ch13[j]=temp2[t];
			j++;t++;
		}
		ch13[j]='\0';
		::no_of_bids = atoi(ch13);
		no_of_bids = atoi(ch13);
		
		std::cout << "here4\n";

		in >> temp3;
		t = 0;
		j = 0;
		char ch14[max];
		while(temp3[t]!='\0')
		{
			ch14[j]=temp3[t];
			j++;t++;
		}
		ch14[j]='\0';
		::no_of_companies = atoi(ch14);
		no_of_companies = atoi(ch14);
		
		std::cout << "here5\n";

		for (int ii=0; ii<no_of_bids; ii++){
			if (k >= no_of_companies){
				cout << "Invalid Input. \n";
				return;
			}
			else{
				bid *b = new bid();
				
				int c_id; 
				in >> c_id;

				float cost;
				in >> cost;

				b->add_company(c_id);
				b->set_cost(cost);
				
				char c;
				in.get(c);

				string a;
				getline(in, a);
				//cout<< "!" << a << "!";

				char ch[max];
				int i=0;int j =0;
				while(a[i] != ('#'))
				{
					while (a[i] != ' '){
						ch[j] = a[i];
						i++;j++;
					}
					ch[j] = '\0';
					b->add_region(atoi(ch));
					i++;
					j = 0;
				}
				getline(in,line);
				cout << "\n";
				bid_map.insert(std::pair<int,bid>(ii,*b));
				if (bid_company.find(c_id) == bid_company.end()){
					company c;
					c.add_bid(ii);
					// bid_company.insert(std::pair<int,company>(c_id,*c));
					bid_company[c_id] = c;
					// delete c;
				}
				else
				{
					bid_company[c_id].add_bid(ii);
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