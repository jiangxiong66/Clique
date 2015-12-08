#include "algo.h"

#include <vector>
#include <sstream>
#include <functional>

#include "graph.h"
#include "mylog.h"
#include "mdatastruct.h"

#define DEBUG

using std::vector;
using std::stringstream;

const double MvcSolver::p_scale = 0.3;
const int MvcSolver::IN_TO_VC = 1;
const int MvcSolver::OUT_FROM_VC = -1;

void MvcSolver::initialize_weight_dscore_stamp(void) {
	fill(dscore.begin(), dscore.end(), 0);
	fill(time_stamp.begin(), time_stamp.end(), 0);
	fill(conf_change.begin(), conf_change.end(), 0);
	fill(edge_weight.begin(), edge_weight.end(), 1);
	for(int i=0; i<e_num; i++) {
		int u = graph.edges[i].from;
		int v = graph.edges[i].to;
		dscore[u] += edge_weight[i];
		dscore[v] += edge_weight[i];
	}
}

void MvcSolver::add_vertex_to_vc(Estack<int>& uncoverd_edges, int v, Eheap<int>* vertex_heap) {
	dscore[v] = -dscore[v];
	is_coverd[v] = 1;
	if(vertex_heap) vertex_heap->remove(v);
	const Node& node = graph.nodes[v];
	int neighbor_num = node.neighbor_nodes.size();
	for(int i=0; i<neighbor_num; i++) {
		int neighbor_v = node.neighbor_nodes[i];
		int neighbor_e = node.neighbor_edges[i];

		if(!is_coverd[neighbor_v]) {
			dscore[neighbor_v] -= edge_weight[neighbor_e];
			conf_change[neighbor_v] = 1;
			uncoverd_edges.remove(neighbor_e);
			if(vertex_heap) vertex_heap->updateWeight(neighbor_v);
		} else {
			dscore[neighbor_v] += edge_weight[neighbor_e];
		}
	}
}

void MvcSolver::remove_vertex_from_vc(Estack<int>& uncoverd_edges, int v, Eheap<int>* vertex_heap) {
	dscore[v] = -dscore[v];
	is_coverd[v] = 0;
	conf_change[v] = 0;
	if(vertex_heap) vertex_heap->push(v);
	const Node& node = graph.nodes[v];
	int neighbor_num = node.neighbor_nodes.size();
	for(int i=0; i<neighbor_num; i++) {
		int neighbor_v = node.neighbor_nodes[i];
		int neighbor_e = node.neighbor_edges[i];

		if(is_coverd[neighbor_v]) {
			dscore[neighbor_v] += edge_weight[neighbor_e];
			conf_change[neighbor_v] = 1;
			uncoverd_edges.push(neighbor_e, neighbor_e);
			if(vertex_heap) vertex_heap->updateWeight(neighbor_v);
		} else {
			dscore[neighbor_v] -= edge_weight[neighbor_e];
		}
	}
}

vector<int> MvcSolver::getMvcGreedly(void) {
	fill(is_coverd.begin(), is_coverd.end(), 0);
	auto int_greater = [](const int& lhs, const int& rhs) {
		return lhs > rhs;
	};
	
	Eheap<int> uncoverd_vertexs(&dscore, int_greater);
	for(int i=1; i<=v_num; i++) {
		uncoverd_vertexs.push(i);
	}

	Estack<int> uncoverd_edges(e_num+1);
	for(int i=0; i<e_num; i++) {
		int value = i;
		int id = i;
		uncoverd_edges.push(value, id);
	}

	vector<int> ans;
	while(!uncoverd_edges.empty()) {
		int best_canditate = uncoverd_vertexs.top();
		
		#ifdef DEBUG
		stringstream os;
		os << "best_canditate : " << best_canditate << " dscore : " << dscore[best_canditate] << endl;
		Log(os);
		#endif
		
		uncoverd_vertexs.pop();
		ans.push_back(best_canditate);
		add_vertex_to_vc(uncoverd_edges, best_canditate, &uncoverd_vertexs);
	}
	return ans;
}

vector<int> MvcSolver::calculateMVC(void) {
	initialize_weight_dscore_stamp();
	vector<int> tmp_mvc = getMvcGreedly();
	
	return tmp_mvc;
}

