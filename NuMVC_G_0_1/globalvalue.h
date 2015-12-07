#ifndef _GLOBAL_VALUE_H
#define _GLOBAL_VALUE_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <set>

#include "statool.h"
#include "mdatastruct.h"

using std::vector;
using std::set;
using std::string;
using std::ofstream;

string fn_main = "main";
vector<string> fn_list{fn_main};
RunMeasurer rm(fn_list);

vector<int> dscore;
vector<int> edge_weight;
vector<int> conf_change;
vector<int> time_stamp;

Eheap<int> v_heap;

int v_num, e_num;
int threshold;
int	c_size;						//cardinality of C

long long	max_steps;			//step limit
int			cutoff_time;			//time limit
long long	step;
int			optimal_size;			//terminate the algorithm before step limit if it finds a vertex cover of optimal_size


ofstream log_os;

void initLog(const char *filename) {
	log_os.open(filename);
}

void endLog(void) {
	log_os.close();
}

void Log(stringstream& stream) {
	log_os << stream.str();
	cout << stream.str();
}

void Log(const string& str) {
	log_os << str;
	cout << str;
}

#endif
