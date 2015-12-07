#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>

#include "statool.h"
#include "graph.h"
#include "globalvalue.h"
#include "algo.h"

using namespace std;

void ansOutput(vector<int>& ans) {
	vector<bool> mis(v_num+1,true);
	for(int x : ans) {
		mis[x] = false;
	}
	vector<int> temp;
	for(int i=1; i<mis.size(); i++) {
		if(mis[i]) temp.push_back(i);
	}
	stringstream os;
	os << "MIS size : " << temp.size() << endl;
	for(int x : temp) os << x << " ";
	os << endl;
	Log(os);
}

int main(int argc, char* argv[]) {
	if (argc < 5) {
		cout << "usage: " << endl;
		return 1;
	}

	rm.BEGINTIME(fn_main);

	Graph graph = readMVCGraph(argv[1]);

	int seed = 0;
	sscanf(argv[2],"%d",&optimal_size);//if you want to stop the algorithm only cutoff time is reached, set optimal_size to 0.
	sscanf(argv[3],"%d",&seed);
	sscanf(argv[4],"%d",&cutoff_time);

	srand(seed);
	v_num = graph.v_num;
	e_num = graph.e_num;
	threshold = (int)(0.5*v_num);
	initLog(argv[5]);

	vector<int> ans = calculateMVC(graph);
	ansOutput(ans);
	//graph.output();

	if(graph.isCoveredBy(ans)) {
		Log("Answer is checked ok!\n");
	} else {
		Log("Wrong Answer\n");
	}
	
	rm.ENDTIME(fn_main);
	Log(rm.getStatistic());
	endLog();
	return 0;
}
