#include "tower_allocation.h"

std::unordered_map<int,bid> bid_map;
std::unordered_map<int,company> bid_company;

float collision_cost = INT_MAX;

float prob_swap = 0.5;
float prob_add = 0.5;
float prob_delete = 0;
float prob_restart = 0.05;

float time_fact = 0.9;
float input_time = 0;

int no_of_bids = 0;
int no_of_companies = 0;
int no_of_regions = 0;

// Function definition for bid class

void bid::set_cost(float cost_in) {
	cost = cost_in;
}

void bid::add_region(int reg_no) {
	set_of_regions.push_back(reg_no);
}

void bid::add_company(int c_id) {
	company = c_id;
}

// Function definition for company class

void company::add_bid(int bid_id) {
	list_of_bids.push_back(bid_id);
}

// Function definition for state class 

float state::get_cost() {

	float state_cost = 0;
	float total_regns = 0;

	std::unordered_map<int, std::vector<int>> map;

	for (auto i = list_of_bids.begin(); i != list_of_bids.end(); ++i) {
		int cost = bid_map[*i].cost;
		state_cost += cost;
		total_regns += bid_map[*i].set_of_regions.size();
	}

	state_cost+= (no_of_regions - total_regns)*(1);

	return state_cost;
}

float state::get_actual_cost() {
	float state_cost = 0;

	std::unordered_map<int, std::vector<int>> map;

	for (auto i = list_of_bids.begin(); i != list_of_bids.end(); ++i) {
		int cost = bid_map[*i].cost;
		state_cost += cost;
	}

	return state_cost;
}