#include "tower_allocation.h"

using namespace std;

std::unordered_map<int,bid>::iterator mit;
std::vector<int> bid_ids;
std::vector<int>::iterator bid_it;

struct compare {
	
	bool operator() (const int& a, const int& b) {
		return (bid_map[a].cost/bid_map[a].set_of_regions.size()) < (bid_map[b].cost/bid_map[b].set_of_regions.size());
	}
};

int main(int argc, char * argv[]) {
	
	allocate a;
	char* infile;
	infile = argv[1];
	char* outfile;
	outfile = argv[2];
	a.input(infile);

	std::srand(std::time(NULL));
	::mit = bid_map.begin();

	std::random_shuffle(bid_ids.begin(),bid_ids.end());
	bid_it = bid_ids.begin();

	state initial_state = a.get_random_state();
	a.greedy_random_search(input_time,initial_state);
	a.output(a.best_state,outfile); 

	return 0;
}