#include <string>
#include <sstream>
#include <cstdio>
#include "tower_allocation.h"

using namespace std;

float random_fraction() {

	static default_random_engine generator (std::time(NULL));	
  	static uniform_real_distribution<float> distribution(0.0,1.0);

  	float num = distribution(generator);

  	return (float)num;
}

void output_state_details(state s) {
	
	cout << "Cost: " << s.get_actual_cost() << "\n";
	cout << "H Cost: " << s.get_cost() << "\n";

	for (auto i = s.list_of_bids.begin(); i != s.list_of_bids.end(); ++i) {
		std::cout<<*i<<" ";
	}

	cout << std::endl;
}

bool check_collision(std::vector<int> v, int new_bid) {
	
	bool* arr = new bool[no_of_regions];

	std::fill(arr, arr+no_of_regions, false);

	if (new_bid >= 0)
		for (auto i = bid_map[new_bid].set_of_regions.begin(); i != bid_map[new_bid].set_of_regions.end(); ++i) {
			arr[*i] = true;
		}	

	for (auto i = v.begin(); i != v.end(); ++i) {
		for (auto j = bid_map[*i].set_of_regions.begin(); j != bid_map[*i].set_of_regions.end(); ++j) {	
			if(arr[*j]){
				delete arr;
				return false;
			}
			else
				arr[*j] = true;
		}
	}

	delete arr;
	return true;
}

std::unordered_map<int,bool> start_state_occur;


state allocate::get_start_state() {
	state new_state;

	std::unordered_map<int, bool> map; // to check whether a company has occurred
	
	int bid_num = *bid_it;
	bid_it++;
	
	new_state.list_of_bids.push_back(bid_num);
	map[bid_map[bid_num].company] = false;

	for (int i = 0; i < no_of_companies; ++i) {
		if(map.find(i) == map.end())
			new_state.remaining_companies.push_back(i);
	}

	if (best_state.list_of_bids.size() == 0)
		best_state = new_state;

	else if (best_state.get_cost() <= new_state.get_cost())
		best_state = new_state;

	return new_state;
}

state allocate::get_random_state() {

	state new_state;
	int count =0;
	int loop_max = (int)(random_fraction()*((float)((no_of_companies-1)/2)));
	bool* arr = new bool[loop_max];
	bool* comp = new bool[no_of_companies];

	std::fill(arr, arr+loop_max, false);
	std::fill(comp, comp+no_of_companies, false);

	for (int i=0; i<loop_max; i++) {
		int bid_num = random_fraction()*no_of_bids;
		while ((arr[bid_num % loop_max] || !check_collision(new_state.list_of_bids,bid_num))&&(count <= 5*no_of_bids)){
			bid_num = random_fraction()*no_of_bids;
			count ++;
		}

		if(!comp[bid_map[bid_num].company] && count <= 5*no_of_companies) {
			arr[bid_num %loop_max] = true;
			new_state.list_of_bids.push_back(bid_num);
			comp[bid_map[bid_num].company] = true;	
		}

		if (count > 5*no_of_bids)
			break;
		count++;
	}

	for (int i = 0; i < no_of_companies; ++i){
		if (!comp[i])
			new_state.remaining_companies.push_back(i);	
	}

	if (best_state.list_of_bids.size() == 0)
		best_state = new_state;

	else if (best_state.get_cost() <= new_state.get_cost())
		best_state = new_state;

	delete arr;
	delete comp;
	return new_state;
}

void allocate::greedy_search(float inp_time, state start_state) {

	state current = start_state; //initially

	// output_state_details(start_state);

	time_t start = time(0);

	while (difftime(time(0), start) <= time_fact * inp_time*60) {
		
		float fract = random_fraction();
		state newstate = current;
		float max_assign_cost = current.get_cost();
		state max_assign_state = current;
		bool done = false;

		for (auto i = current.list_of_bids.begin(); i != current.list_of_bids.end(); ++i){
			
			newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	
			int comp_id = bid_map[*i].company;
			
			for (auto j = bid_map.begin(); j != bid_map.end() && (((j->second).company == comp_id) || (find(current.remaining_companies.begin(),current.remaining_companies.end(),(j -> second).company) != current.remaining_companies.end())); ++j){
				
				auto it = find(current.remaining_companies.begin(), current.remaining_companies.end(), (j -> second).company);
				if((*i != (j->first)) && (((j -> second).company == comp_id) || it != current.remaining_companies.end())) {
					newstate.list_of_bids.push_back((j->first));
					if (newstate.get_cost() >= max_assign_cost) {
						max_assign_state = newstate;
						max_assign_state.remaining_companies.erase(remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (bid_map[(j->first)].company)), max_assign_state.remaining_companies.end());	
						max_assign_cost = max_assign_state.get_cost();
						done = true;
					}
					newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((j->first))), newstate.list_of_bids.end());
				}
			}
			newstate.list_of_bids.push_back(*i);
		}

		for (auto i = current.remaining_companies.begin(); i != current.remaining_companies.end(); ++i) {
			for (auto j = bid_company[*i].list_of_bids.begin(); j != bid_company[*i].list_of_bids.end(); ++j) {
				newstate.list_of_bids.push_back((*j));
				if (newstate.get_cost() >= max_assign_cost) {
					max_assign_state = newstate;
					max_assign_state.remaining_companies.erase(remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), bid_map[*j].company), max_assign_state.remaining_companies.end());	
					max_assign_cost = max_assign_state.get_cost();
					done = true;
				}
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((*j))), newstate.list_of_bids.end());	
			}
		}
			
		if(best_state.get_cost() <= max_assign_state.get_cost())
			best_state = max_assign_state;

		current = max_assign_state;

		if(!done)
			current = get_start_state();
		
		//------------------------DISPLAY-----------------------------------------------
		// // std::cout<<"best\n";
		// // output_state_details(best_state);
		// // std::cout<<"current\n";
		// // output_state_details(current);
	}
}

void allocate::greedy_random_search(float inp_time, state start_state) {
	
	state current = start_state; //initially

	// output_state_details(start_state);

	time_t start = time(0);

	while(difftime(time(0), start) <= time_fact * inp_time*60) {

		float fract = random_fraction();

		state newstate = current;
		float max_assign_cost = current.get_cost();
		state max_assign_state = current;
		bool done = false;

		if (fract <= prob_swap+prob_add) {
			
			for (auto i = current.list_of_bids.begin(); i != current.list_of_bids.end(); ++i){
				
				newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	
				int comp_id = bid_map[*i].company;
				
				//for same company
				for (auto j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end(); ++j) {
					if(check_collision(newstate.list_of_bids,*j)) {
						newstate.list_of_bids.push_back((*j));
						if (newstate.get_cost() >= max_assign_cost - 1000) {
							max_assign_state = newstate;
							max_assign_cost = max_assign_state.get_cost();
							done = true;
						}
						newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((*j))), newstate.list_of_bids.end());	
					}
				}

				for (auto j = newstate.remaining_companies.begin(); j != newstate.remaining_companies.end(); ++j) {
					for (auto k = bid_company[*j].list_of_bids.begin(); k != bid_company[*j].list_of_bids.end(); ++k) {
						
						if(check_collision(newstate.list_of_bids,*k)) {
							newstate.list_of_bids.push_back((*k));
							if (newstate.get_cost() >= max_assign_cost) {
								max_assign_state = newstate;
								max_assign_state.remaining_companies.erase(remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (*j)), max_assign_state.remaining_companies.end());	
								max_assign_state.remaining_companies.push_back(comp_id);
								max_assign_cost = max_assign_state.get_cost();
								done = true;
							}

							if(check_collision(newstate.list_of_bids,*i)) {
								newstate.list_of_bids.push_back(*i);
								if (newstate.get_cost() >= max_assign_cost){
									max_assign_state = newstate;
									max_assign_state.remaining_companies.erase(remove(max_assign_state.remaining_companies.begin(), max_assign_state.remaining_companies.end(), (*j)), max_assign_state.remaining_companies.end());	
									max_assign_cost = max_assign_state.get_cost();
									done = true;
								}
								newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((*i))), newstate.list_of_bids.end());	
							}
							newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), ((*k))), newstate.list_of_bids.end());	
						}
					}
				}
				newstate.list_of_bids.push_back(*i);
			}

			if (best_state.get_cost() <= max_assign_state.get_cost())
				best_state = max_assign_state;

			if (current.get_cost() != max_assign_state.get_cost())
				current = max_assign_state;
			else
				current = get_random_state();
			
			if (!done)
				current = get_random_state();
		}
		else{
			current = get_random_neighbour(current);
		}

		//------------------------DISPLAY-----------------------------------------------
		// std::cout<<"best\n";
		// output_state_details(best_state);
		// std::cout<<"current\n";
		// output_state_details(current);
	}
}

state get_random_neighbour(state temp) {
	
	state input = temp;
	if (random_fraction() <= 0.5){

		int index = (int)((random_fraction())*(input.list_of_bids.size() - 1));
		int bid_no = input.list_of_bids[index];

		input.list_of_bids.erase(std::remove(input.list_of_bids.begin(), input.list_of_bids.end(), bid_no), input.list_of_bids.end());
		bool done = false;

		for (auto i = bid_company[bid_map[bid_no].company].list_of_bids.begin(); (i != bid_company[bid_map[bid_no].company].list_of_bids.end()) && !done; ++i){
			if(*i != bid_no && check_collision(input.list_of_bids,*i)){
				input.list_of_bids.push_back(*i);
				if(input.get_cost() >= temp.get_cost()){
					done = true;
					break;
				}
				else{
					input.list_of_bids.erase(remove(input.list_of_bids.begin(), input.list_of_bids.end(), *i), input.list_of_bids.end());			
				}
			}
		}

		if(!done){
			for (auto comp_id = input.remaining_companies.begin(); (comp_id != input.remaining_companies.end())&&(!done); ++comp_id){
				int company_id = *comp_id;
				for (auto i = bid_company[company_id].list_of_bids.begin(); (i != bid_company[company_id].list_of_bids.end()) && (!done); ++i){
					if (check_collision(input.list_of_bids,*i)){
						input.list_of_bids.push_back(*i);
						if(input.get_cost() >= temp.get_cost()){
							input.remaining_companies.erase(remove(input.remaining_companies.begin(), input.remaining_companies.end(), company_id), input.remaining_companies.end());
							input.remaining_companies.push_back(bid_map[bid_no].company);
							done = true;
							break;
						}
						else{
							input.list_of_bids.erase(remove(input.list_of_bids.begin(), input.list_of_bids.end(), *i), input.list_of_bids.end());			
						}
					}
				}			
			}
			random_shuffle(input.remaining_companies.begin(),input.remaining_companies.end());
		}

		if(!done){
			input.list_of_bids.push_back(bid_no);
		}
	}
	else {
		bool done = false;
		for (auto comp_id = input.remaining_companies.begin(); (comp_id != input.remaining_companies.end())&&(!done); ++comp_id){
			int company_id = *comp_id;
			
			for (auto i = bid_company[company_id].list_of_bids.begin(); i != bid_company[company_id].list_of_bids.end(); ++i){
				if(check_collision(input.list_of_bids,*i)){
					input.list_of_bids.push_back(*i);
					if(input.get_cost() >= temp.get_cost()){
						input.remaining_companies.erase(remove(input.remaining_companies.begin(), input.remaining_companies.end(), company_id), input.remaining_companies.end());
						done = true;
						break;
					}
					else{
						input.list_of_bids.erase(remove(input.list_of_bids.begin(), input.list_of_bids.end(), *i), input.list_of_bids.end());			
					}
				}
			}
		}			
		random_shuffle(input.remaining_companies.begin(),input.remaining_companies.end());
	}

	return input;
}

void allocate::sa_search(float inp_time, state start_state){
	
	state current = start_state; //initially

	// output_state_details(start_state);

	time_t start = time(0);

	while (difftime(time(0), start) <= time_fact * inp_time*60) {
		if (random_fraction() <= prob_swap+prob_add){
			float fract = random_fraction();
			state newstate = get_random_neighbour(current);
			float delta = newstate.get_cost() - current.get_cost();
			if (delta >= 0){
				current = newstate;
			}
			else{
				if (fract <= exp(delta/((difftime(time(0), start)*35000+500000)))){
					current = newstate;
				}
			}
			if (best_state.get_cost() <= current.get_cost())
				best_state = current;
		}
		else{
			current = get_random_state();
		}

		//------------------------DISPLAY-----------------------------------------------

		// std::cout<<"best\n";
		// output_state_details(best_state);
		// std::cout<<"current\n";
		// output_state_details(current);
	}
}

void allocate::search(float inp_time, state start_state){
	
	state current = start_state; //initially

	// output_state_details(start_state);

	time_t start = time(0);
	while (difftime(time(0), start) <= time_fact * inp_time*60) {
		float fract = random_fraction();

		if (fract <= prob_swap){
			// code for swap
			// for the time being the swap is from an open company to itself
			state newstate = current;
			bool done = false;

			for (auto i = current.list_of_bids.begin(); i != current.list_of_bids.end() && difftime( time(0), start) <= time_fact * inp_time; ++i){
				newstate.list_of_bids.erase(std::remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());	
				int comp_id = bid_map[*i].company;
				for (auto j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end(); ++j){
					if(*i != *j){
						newstate.list_of_bids.push_back(*j);
						if (newstate.get_cost() >= current.get_cost()){
							current = newstate;
							done = true;
							break;
						}
						else{
							newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
						}
					}
				}

				if(done){
					break;
				}
				else{
					newstate.list_of_bids.push_back(*i);
				}
			}

			if(done){
				if(best_state.get_cost() <= current.get_cost())
					best_state = current;
			}
		}
		else if (fract <= prob_swap+prob_add && fract > prob_swap){
			// code for add
			state newstate = current;
			bool done = false;
			for (auto i = current.remaining_companies.begin(); i != current.remaining_companies.end(); ++i){
				int comp_id = *i;
				for (std::vector<int>::iterator j = bid_company[comp_id].list_of_bids.begin(); j != bid_company[comp_id].list_of_bids.end(); ++j){
					newstate.list_of_bids.push_back(*j);
					if (newstate.get_cost() >= current.get_cost()){
						newstate.remaining_companies.erase(remove(newstate.remaining_companies.begin(), newstate.remaining_companies.end(), comp_id), newstate.remaining_companies.end());	
						current = newstate;
						done = true;
						break;
					}
					else{
						newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*j)), newstate.list_of_bids.end());	
					}
				}

				if(done){
					break;
				}
			}

			if(done){
				if(best_state.get_cost() <= current.get_cost())
					best_state = current;
			}
		}
		else if (fract <= prob_swap+prob_add + prob_delete && fract > prob_swap+prob_add){
			// code for delete
			state newstate = current;
			bool done = false;

			for (auto i = current.list_of_bids.begin(); i != current.list_of_bids.end(); ++i){
				newstate.list_of_bids.erase(remove(newstate.list_of_bids.begin(), newstate.list_of_bids.end(), (*i)), newstate.list_of_bids.end());
				int comp_id = bid_map[*i].company;
				if (newstate.get_cost() >= current.get_cost()){
					newstate.remaining_companies.push_back(comp_id);
					current = newstate;
					done = true;
					break;
				}
				else{
					newstate.list_of_bids.push_back(*i);
				}
			}

			if(done){
				if(best_state.get_cost() <= current.get_cost())
					best_state = current;
			}
		}
		else{
			// random restart
			current = get_start_state();	
		}

		// std::cout<<"best\n";
		// output_state_details(best_state);
		// std::cout<<"current\n";
		// output_state_details(current);
	}
}

vector<string> split(string str, char delimiter){
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;
	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

void allocate::input(char filename[]) {
	string temp,temp1,temp2,temp3,line;
	int no_of_regions, no_of_bids, no_of_companies;
	int k = 0;
	ifstream in;
	in.open(filename);

	if (!in.is_open()){
		cout << "File not found.\n";
	}
	else {
		in >> temp;
		int t=0;int j=0;
		char ch11[max];

		while (temp[t]!='\0'){
			ch11[j]=temp[t];
			j++;t++;
		}
		ch11[j]='\0';
		::input_time = (strtod (ch11, NULL));
		
		in >> temp1;
		t = 0;
		j = 0;
		char ch12[max];
		while(temp1[t]!='\0'){
			ch12[j]=temp1[t];
			j++;
			t++;
		}
		ch12[j]='\0';
		::no_of_regions = atoi(ch12);
		no_of_regions = atoi(ch12);
		
		in >> temp2;
		t = 0;
		j = 0;

		char ch13[max];
		while(temp2[t]!='\0'){
			ch13[j]=temp2[t];
			j++;t++;
		}
		ch13[j]='\0';
		::no_of_bids = atoi(ch13);
		no_of_bids = atoi(ch13);
		
		in >> temp3;
		t = 0;
		j = 0;
		char ch14[max];
		while(temp3[t]!='\0'){
			ch14[j]=temp3[t];
			j++;t++;
		}
		ch14[j]='\0';
		::no_of_companies = atoi(ch14);
		no_of_companies = atoi(ch14);

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
				
				char ch[max];
				int i=0;int j =0;
				while(a[i] != ('#')){
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
					bid_company[c_id] = c;
				}
				else{
					bid_company[c_id].add_bid(ii);
				}
				delete b;

				bid_ids.push_back(ii);
			}
		}
	}
}

void allocate::output(state output_state, char filename[]){
	
	ofstream out(filename, ios::out);
	vector<int> list_of_bids = output_state.list_of_bids;
	for (int i=0; i<list_of_bids.size(); i++){
		out << list_of_bids.at(i) << " ";
	}
	out << "#";
	out.close();
}