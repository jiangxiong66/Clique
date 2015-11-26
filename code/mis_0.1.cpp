#include <vector>
#include <list>
#include <queue>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <random>
#include <map>

#include "statool.h"

#define UNDEBUG

using namespace std;

struct Edge {
	int from, to, weight;
	Edge(int _from, int _to, int _weight): from(_from), to(_to), weight(_weight) {
	};
};

struct Node {
	int id;
	int degree;
	int weight;
	Node(): id(0), degree(0), weight(0) {};
	Node(int _id, int _degree, int _weight=0): id(_id), degree(_degree), weight(_weight) {};
	bool operator < (const Node& other) const {
		//if(degree != other.degree) return degree < other.degree;
		return weight < other.weight;
	}
};

vector<Edge> edges;
vector< vector<int> > gmat;
vector< vector<int> > glist;

int node_num, edge_num;

ofstream log_os;
stringstream mcout;

int mis_number;
int target_mis = 100;
int iterate_time;
int iterate_count = 0;
int iterate_limit = 100;
int rollout_ratio = 4;
int log_pre_iterate = 1000;
int cost_precision = 2;
int cost_range = 5;

void OutputStatistic(void);
map<string, string> readConfig(const string& cfgfile);
void init(const map<string, string>& cfg);

void Log(const string& str);
void Log(stringstream& stream);
double randomCost(void);
bool checkAndLog(vector<Node>& temp_mis, int k);
void readGraph(const char *filename);

bool isIndependentTo(vector<Node> &temp_mis, int u);
void renewCanditate(vector<Node>& temp_mis, vector<Node>& canditate);
void rollElementOut(vector<Node>& temp_mis, vector<Node>& canditate);

void enlargeTempMis(vector<Node>& temp_mis, vector<Node>& canditate);

void mis(int k);

string fn_isIndependenTo = "isIndependenTo";
string fn_renewCanditate = "renewCanditate";
string fn_rollElementOut = "rollElementOut";
string fn_enlargeTempMis = "enlargeTempMis";
string fn_main = "main";

vector<string> fn_list{fn_isIndependenTo, fn_renewCanditate, fn_rollElementOut, fn_enlargeTempMis, fn_main};

RunMeasurer rm(fn_list);

int main(int args, char *argv[]) {
	rm.BEGINTIME(fn_main);
	string cfgfile;
	cout << "please input test config filename: ";
	cin >> cfgfile;
	map<string,string> cfg = readConfig(cfgfile);
	init(cfg);
	
	mis(target_mis);
	
	rm.ENDTIME(fn_main);
	
	Log(rm.getStatistic());
	return 0;
}

map<string, string> readConfig(const string& cfgfile) {
	map<string, string> cfg;
	ifstream stream(cfgfile);
	string key, value;
	while(stream >> key >> value) {
		cfg[key] = value;
	}
	return cfg;
}

void init(const map<string, string>& cfg) {
	string filename;
	string version = "";
	for(auto it = cfg.begin(); it != cfg.end(); ++it) {
		if(it->first == "filename:") filename = it->second;
		else if(it->first == "version:") version = it->second;
		else if(it->first == "target_mis:") target_mis = stoi(it->second);
		else if(it->first == "iterate_limit:") iterate_limit = stoi(it->second);
		else if(it->first == "rollout_ratio:") rollout_ratio = stoi(it->second);
		else if(it->first == "cost_range:") cost_range = stoi(it->second);
		else if(it->first == "cost_precision:") cost_precision = stoi(it->second);
	}
	log_os.open(filename+".log"+version);
	readGraph(filename.c_str());
}

void Log(stringstream& stream) {
	log_os << stream.str();
	cout << stream.str();
}

void Log(const string& str) {
	log_os << str;
	cout << str;
}

double randomCost(void) {
	return (double)(rand()%cost_range)/cost_precision;
}

void outputSol(vector<Node>& temp_mis) {
	stringstream stream;
	stream << "* Solution Found : " << mis_number << endl;
	for(auto node : temp_mis ) {
		stream << node.id + 1 << " ";
	}
	stream << endl;
	Log(stream.str());
}

bool checkAndLog(vector<Node>& temp_mis, int k) {
	stringstream stream;
	sort(temp_mis.begin(), temp_mis.end(), [](const Node& lhs, const Node& rhs){return lhs.id<rhs.id;});
	if(iterate_time%log_pre_iterate==0) {
		stream << iterate_time << " iterate run." << endl;
		Log(stream);
	}
	
	#ifdef DEBUG
	outputSol(temp_mis);
	#endif
	
	if(temp_mis.size() > mis_number) {
		mis_number = temp_mis.size();
		outputSol(temp_mis);
	}
	
	return (mis_number >= k || iterate_count >= iterate_limit);
}

void readGraph(const char *filename) {
	ifstream input;
	input.open(filename);
	
	string str;
	input >> str >> str >> node_num >> edge_num;
	gmat.resize(node_num, vector<int>(node_num,0));
	glist.resize(node_num, vector<int>());
	
	for( int i=0; i<edge_num; i++ ) {
		int from, to;
		input >> str >> from >> to;

		from--,to--;
		
		gmat[from][to] = 1;
		gmat[to][from] = 1;
		glist[from].push_back(to);
		glist[to].push_back(from);
		edges.push_back(Edge{from, to, 1});
	}
}

bool isIndependentTo(vector<Node> &temp_mis, int u) {
	rm.BEGINTIME(fn_isIndependenTo);

	bool flag = true;
	for( auto node : temp_mis ) {
		int v = node.id;
		if(gmat[u][v]) {
			flag = false;
			break;
		}
	}

	rm.ENDTIME(fn_isIndependenTo);
	return flag;
}

void renewCanditate(vector<Node>& temp_mis, vector<Node>& canditate) {
	rm.BEGINTIME(fn_renewCanditate);

	vector<int> tdict(node_num,0);
	for( auto node : temp_mis ) {
		tdict[node.id] = 1;
	}
	for(auto it = canditate.rbegin(); it!=canditate.rend(); ++it) {
		if(tdict[it->id]) {
			swap(*it, *canditate.rbegin());
			canditate.pop_back();
		}
	}

	rm.ENDTIME(fn_renewCanditate);
}

void enlargeTempMis(vector<Node>& temp_mis, vector<Node>& canditate) {
	rm.BEGINTIME(fn_enlargeTempMis);

	for( auto temp : canditate ) {
		if(isIndependentTo(temp_mis, temp.id)) {
			temp_mis.push_back(temp);
		}
	}

	renewCanditate(temp_mis, canditate);

	rm.ENDTIME(fn_enlargeTempMis);
}

void rollElementOut(vector<Node>& temp_mis, vector<Node>& canditate) {
	rm.BEGINTIME(fn_rollElementOut);

	random_shuffle(temp_mis.begin(), temp_mis.end());
	int roll_num = temp_mis.size() / rollout_ratio;
	for(int i=0; i<roll_num; ++i) {
		Node node = temp_mis.back();
		temp_mis.pop_back();
		
		node.weight += randomCost();
		canditate.push_back(node);
	}

	rm.ENDTIME(fn_rollElementOut);
}

void mis(int k) {
	vector<Node> temp_mis;
	vector<Node> canditate(node_num);
	for(int i=0; i<node_num; i++) {
		canditate[i] = Node(i, glist[i].size());
	}
	while(true) {
		++iterate_time;
		++iterate_count;
		enlargeTempMis(temp_mis, canditate);
		//refineByExchange(temp_mis, canditate);
		if(checkAndLog(temp_mis, k)) break;
		rollElementOut(temp_mis, canditate);
	}
}
