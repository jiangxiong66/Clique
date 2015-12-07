#ifndef ALGO_H
#define ALGO_H

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>

#include "statool.h"
#include "graph.h"
#include "globalvalue.h"
#include "mdatastruct.h"

using std::vector;

vector<int> init_sol(const Graph& graph) {
	conf_change = vector<int>(v_num+1, 1);
	time_stamp = vector<int>(v_num+1, 0);
	edge_weight = vector<int>(e_num, 1);
	dscore = vector<int>(v_num+1, 0);
	v_heap = Eheap<int>(&dscore, [](const int& lhs, const int& rhs)->bool{ return lhs > rhs;});
	

	for( int e_id=0; e_id<graph.edges.size(); e_id++ ) {
		int u = graph.edges[e_id].from;
		int v = graph.edges[e_id].to;
		dscore[u] += edge_weight[e_id];
		dscore[v] += edge_weight[e_id];
	}

	Elist<int> uncoverd_edges;
	for(int i=0; i<e_num; i++) {
		uncoverd_edges.add(i);
	}

	for(int i=1; i<=v_num; i++) {
		v_heap.insert(i);
	}

	vector<bool> is_coverd(v_num+1,false);
	vector<int> v;

	while(!uncoverd_edges.empty()) {
		int best_v = v_heap.top();
		
		stringstream os;
		os << "best_v : " << best_v << " dscore : " << dscore[best_v] << endl;
		Log(os);
		
		v_heap.pop();
		is_coverd[best_v] = true;
		dscore[best_v] = -dscore[best_v];
		v.push_back(best_v);
		
		const Node& node = graph.nodes[best_v];
		for(int i=0; i<node.neighbor_nodes.size(); i++) {
			int nid = node.neighbor_nodes[i];
			int eid = node.neighbor_edges[i];
			if(!is_coverd[nid]) {
				v_heap.remove(nid);
				dscore[nid] -= edge_weight[eid];
				conf_change[nid] = 1;
				v_heap.insert(nid);
				uncoverd_edges.cover(eid);
			} else {
				dscore[nid] += edge_weight[eid];
			}
		}
	}
	
	return v;
}

vector<int> calculateMVC(const Graph& graph) {
	vector<int> tmp_mvc = init_sol(graph);
	return tmp_mvc;
}

#endif
