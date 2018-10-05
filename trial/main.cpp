#include "tower_allocation.h"
using namespace std;

std::map<int,bid>::iterator mit;
std::vector<int> bid_ids;
std::vector<int>::iterator bid_it;

int main(){
	allocate a;
	a.input();

	// std::srand(std::time(0));
	std::srand(std::time(NULL));
	::mit = bid_map.begin();

	std::random_shuffle(bid_ids.begin(),bid_ids.end());

	bid_it = bid_ids.begin();

	std::cout << "input taken\n"; 
	std::cout << bid_map.size() << ":map size" << std::endl;
	for (std::map<int,bid>::iterator i = bid_map.begin(); i != bid_map.end(); ++i)
	{
		std::cout<< i->first << ",yo\n";
	}

	std::cout << std::endl;

	for (std::map<int,company>::iterator i = bid_company.begin(); i != bid_company.end(); ++i)
	{
		std::cout<< i->first << ":";

		for (std::vector<int>::iterator j = (i->second).list_of_bids.begin(); j != (i->second).list_of_bids.end(); ++j)
		{
			std::cout<<*j<<",";
		}

		std::cout<<std::endl;
	}

	std::cout << "inp_time:" <<input_time * time_fact<< std::endl;
	state initial_state = a.get_start_state();
	std::cout << "start state taken\n";
	// a.greedy_search(input_time,initial_state);
	a.greedy_random_search(input_time,initial_state);
	std::cout << "searched\n";
	a.output(a.remove_collision(a.best_state)); 

	return 0;
}