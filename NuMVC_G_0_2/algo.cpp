#include "algo.h"

#include <vector>
#include <limits>
#include <sstream>
#include <functional>

#include "graph.h"
#include "mylog.h"
#include "mdatastruct.h"

#define DEBUG

using std::vector;
using std::stringstream;

const double MvcSolver::p_scale = 0.3;
const int MvcSolver::try_step = 100;
const int MvcSolver::IN_TO_VC = 1;
const int MvcSolver::OUT_FROM_VC = -1;

namespace algo{
	auto int_greater = [](const int& lhs, const int& rhs) {
	return lhs > rhs;
	};

	auto dscore_greater = [](const NodeWType& lhs, const NodeWType& rhs) {
		return lhs.dscore > rhs.dscore;
	};

	auto node_greater = [](const NodeWType& lhs, const NodeWType& rhs) {
		if(lhs.conf_change != rhs.conf_change) return lhs.conf_change < rhs.conf_change;
		if(lhs.dscore != rhs.dscore) return lhs.dscore > lhs.dscore;
		return lhs.time_stamp < rhs.time_stamp;
	};
}

vector<int> MvcSolver::get_mvc_vector(void) {
	vector<int> tmp_mvc;
	for(int i=1; i<=v_num; i++) {
		if(is_coverd[i]) tmp_mvc.push_back(i);
	}
	return tmp_mvc;
}

void MvcSolver::initialize_weight_dscore_stamp(void) {
	node_weight = vector<NodeWType>(v_num+1);
	edge_weight = vector<int>(e_num+1, 1);
	for(int i=0; i<e_num; i++) {
		int u = graph.edges[i].from;
		int v = graph.edges[i].to;
		node_weight[u].dscore += edge_weight[i];
		node_weight[v].dscore += edge_weight[i];
	}
}

void MvcSolver::add_vertex_to_vc(Estack<int>& uncoverd_edges, int v, Eheap<NodeWType>* vertex_heap) {
	node_weight[v].dscore = -node_weight[v].dscore;
	is_coverd[v] = 1;
	if(vertex_heap) vertex_heap->remove(v);
	const Node& node = graph.nodes[v];
	int neighbor_num = node.neighbor_nodes.size();
	for(int i=0; i<neighbor_num; i++) {
		int neighbor_v = node.neighbor_nodes[i];
		int neighbor_e = node.neighbor_edges[i];

		if(!is_coverd[neighbor_v]) {
			node_weight[neighbor_v].dscore -= edge_weight[neighbor_e];
			node_weight[neighbor_v].conf_change = 1;
			uncoverd_edges.remove(neighbor_e);
			if(vertex_heap) vertex_heap->updateWeight(neighbor_v);
		} else {
			node_weight[neighbor_v].dscore += edge_weight[neighbor_e];
		}
	}
}

void MvcSolver::remove_vertex_from_vc(Estack<int>& uncoverd_edges, int v, Eheap<NodeWType>* vertex_heap) {
	node_weight[v].dscore = -node_weight[v].dscore;
	is_coverd[v] = 0;
	node_weight[v].conf_change = 0;
	if(vertex_heap) vertex_heap->push(v);
	const Node& node = graph.nodes[v];
	int neighbor_num = node.neighbor_nodes.size();
	for(int i=0; i<neighbor_num; i++) {
		int neighbor_v = node.neighbor_nodes[i];

		int neighbor_e = node.neighbor_edges[i];
		int e_u = graph.edges[neighbor_e].from;
		int e_v = graph.edges[neighbor_e].to;
		//if(is_coverd[e_u] + is_coverd[e_v] == 0) uncoverd_edges.push(neighbor_e, neighbor_e);

		if(!is_coverd[neighbor_v]) {
			node_weight[neighbor_v].dscore += edge_weight[neighbor_e];
			node_weight[neighbor_v].conf_change = 1;
			uncoverd_edges.push(neighbor_e, neighbor_e);
			if(vertex_heap)  {
				vertex_heap->updateWeight(neighbor_v);
			}
		} else {
			node_weight[neighbor_v].dscore -= edge_weight[neighbor_e];
		}
	}
}

vector<int> MvcSolver::getMvcGreedly(void) {
	fill(is_coverd.begin(), is_coverd.end(), 0);
	
	Eheap<NodeWType> uncoverd_vertexs(&node_weight, algo::dscore_greater);
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
		os << "best_canditate : " << best_canditate << " dscore : " << node_weight[best_canditate].dscore << endl;
		Log(os);
		#endif
		
		uncoverd_vertexs.pop();
		ans.push_back(best_canditate);
		add_vertex_to_vc(uncoverd_edges, best_canditate, &uncoverd_vertexs);
	}
	return ans;
}

int MvcSolver::max_dscore_coverd_vertex(void) {
	int max_dscore = std::numeric_limits<int>::min();
	int midx = 0;
	for(int i=1; i<=v_num; i++) {
		if(is_coverd[i] && node_weight[i].dscore>max_dscore) {
			max_dscore = node_weight[i].dscore;
			midx = i;
		}
	}
	return midx;
}

int MvcSolver::earliest_max_dscore_coverd_vertex(int tabu_v) {
	int best_canditate = 0;
	for(int i=1; i<=v_num; i++) {
		if(is_coverd[i]) {
			best_canditate = i;
			break;
		}
	}
	for(int i=best_canditate+1; i<=v_num; i++) {
		if(!is_coverd[i] || i==tabu_v) continue;
		if(node_weight[i].dscore < node_weight[best_canditate].dscore) continue;
		if(node_weight[i].dscore > node_weight[best_canditate].dscore) {
			best_canditate = i;
		} else {
			if(node_weight[i].time_stamp < node_weight[best_canditate].time_stamp) best_canditate = i;
		}
	}
	return best_canditate;
}

int MvcSolver::luckly_uncoverd_vertex(Estack<int> uncoverd_edges) {
	int e_id = uncoverd_edges[rand()%uncoverd_edges.size()];
	int u = graph.edges[e_id].from;
	int v = graph.edges[e_id].to;
	if(node_weight[u].conf_change) return u;
	if(node_weight[v].conf_change) return v;
	if(node_weight[u].dscore > node_weight[v].dscore) return u;
	if(node_weight[u].dscore < node_weight[v].dscore) return v;
	if(node_weight[u].time_stamp < node_weight[v].time_stamp) return u;
	return v;
}

void MvcSolver::forget_edge_weights(void) {
	int new_total_weight = 0;
	for(auto &x : node_weight) {
		x.dscore = 0;
	}
	
	for(int i=0; i<e_num; i++) {
		edge_weight[i] *= p_scale;
		new_total_weight += edge_weight[i];

		int u = graph.edges[i].from;
		int v = graph.edges[i].to;
		if(is_coverd[u] + is_coverd[v] == 0) {
			node_weight[u].dscore += edge_weight[i];
			node_weight[v].dscore += edge_weight[i];
		}
		if(is_coverd[u] + is_coverd[v] == 1) {
			if(is_coverd[u]) node_weight[u].dscore -= edge_weight[i];
			else node_weight[v].dscore -= edge_weight[i];
		}
	}
	ave_weight = new_total_weight / e_num;
}

void MvcSolver::updateWeight(Estack<int>& uncoverd_edges) {
	int uncoverd_edge_num = uncoverd_edges.size();
	for(int i=0; i<uncoverd_edge_num; i++) {
		int e_id = uncoverd_edges[i];
		Edge e = graph.edges[e_id];
		++edge_weight[e_id];
		++node_weight[e.from].dscore;
		++node_weight[e.to].dscore;
	}
	delta_total_weight += uncoverd_edge_num;
	if(delta_total_weight >= e_num) {
		++ave_weight;
		delta_total_weight -= e_num;
	}
	if(ave_weight >= threshold) {
		forget_edge_weights();
	}
}

vector<int> MvcSolver::calculateMVC(void) {
	initialize_weight_dscore_stamp();
	vector<int> ans = getMvcGreedly();
	
	Estack<int> uncoverd_edges(e_num+1);
	Eheap<NodeWType> uncoverd_vertexs(&node_weight, algo::dscore_greater);
	Eheap<NodeWType> coverd_vertexs(&node_weight, algo::node_greater);
	fill(is_coverd.begin(), is_coverd.end(), 0);
	for(auto x : ans) {
		is_coverd[x] = 1;
	}
	for(int i=1; i<is_coverd.size(); i++) {
		if(!is_coverd[i]) uncoverd_vertexs.push(i);
		else coverd_vertexs.push(i);
	}

	int step = 1;
	delta_total_weight = 0, ave_weight = 0;
	auto start = steady_clock::now();

	while(true) {
		if(uncoverd_edges.empty()) {
			//Log("uncoverd_edges empty.\n");
			vector<int> tmp_mvc = get_mvc_vector();
			
			if(tmp_mvc.size() < ans.size())  ans = tmp_mvc;
			
			//if(v_num-ans.size() > 85) return ans;
			
			OutputCoMvc(ans, v_num);
			//cout << "---BEFORE---" << endl;
			cout << "answer size : " << ans.size() << endl;
			//cout << "uncoverd_edges : " << uncoverd_edges.size() << endl;
			//cout << "uncoverd_vertexs : " << uncoverd_vertexs.size() << endl;
			int best_remove_v = max_dscore_coverd_vertex();
			remove_vertex_from_vc(uncoverd_edges, best_remove_v, &uncoverd_vertexs);
			
			//cout << "---AFTER----" << endl;
			//cout << "uncoverd_edges : " << uncoverd_edges.size() << endl;
			//cout << "uncoverd_vertexs : " << uncoverd_vertexs.size() << endl;
			//cout << "to be continue.\n";
			continue;
		}

		if(step % try_step == 0) {
			auto finish = steady_clock::now();
			double elap_time = duration_cast<duration<double>>(finish-start).count();
			if(elap_time >= cutoff_time) break;
		}
		
		//cout << "uncoverd edges : " << uncoverd_edges.size() << endl;
		
		int takein_canditate = luckly_uncoverd_vertex(uncoverd_edges);
		add_vertex_to_vc(uncoverd_edges, takein_canditate, &uncoverd_vertexs);
		
		int remove_canditate = earliest_max_dscore_coverd_vertex(takein_canditate);
		remove_vertex_from_vc(uncoverd_edges, remove_canditate, &uncoverd_vertexs);

		updateWeight(uncoverd_edges);

		step++;
	}
	stringstream msg;
	msg << "step : " << step << endl;
	Log(msg);
	return ans;
}
