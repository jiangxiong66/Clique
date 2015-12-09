#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>

//#include "statool.h"
#include "graph.h"
#include "mylog.h"
#include "algo.h"
#include "mdatastruct.h"

using namespace std;

string fn_main = "main";
vector<string> fn_list{fn_main};
RunMeasurer rm(fn_list);

int main(int argc, char* argv[]) {
	if (argc < 5) {
		cout << "usage: " << endl;
		return 1;
	}

	rm.BEGINTIME(fn_main);

	int seed = 0;
	int optimal_size;
	int cutoff_time;
	sscanf(argv[2],"%d",&optimal_size);//if you want to stop the algorithm only cutoff time is reached, set optimal_size to 0.
	sscanf(argv[3],"%d",&seed);
	sscanf(argv[4],"%d",&cutoff_time);
	initLog(argv[5]);

	Graph graph = readMVCGraph(argv[1]);
	MvcSolver mvc_solver(graph);
	vector<int> ans = mvc_solver.solveMvc(cutoff_time, optimal_size, 0.5, seed);
	OutputCoMvc(ans, graph.v_num);
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

