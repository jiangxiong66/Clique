#ifndef ALGO_H
#define ALGO_H

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstddef>

#include "graph.h"
#include "mdatastruct.h"

using std::vector;

struct NodeWType{
	int dscore;
	int conf_change;
	int time_stamp;
	NodeWType(int _dscore=0, int _time_stamp=0, int _conf_change=0):
		dscore(_dscore),
		time_stamp(_time_stamp),
		conf_change(_conf_change) {};
};

class MvcSolver
{
private:
	long long	step;
	long long	max_steps;			//step limit
	int			cutoff_time;		//time limit
	int			optimal_size;
	int 		threshold;

private:
	const Graph graph;
	const int v_num, e_num;
	vector<int> edge_weight;
	vector<NodeWType> node_weight;

private:
	static const double p_scale;
	static const int try_step;
	static const int IN_TO_VC;
	static const int OUT_FROM_VC;

private:
	int delta_total_weight, ave_weight;
	vector<int> is_coverd;

	vector<int> getMvcGreedly(void);
	vector<int> calculateMVC(void);
	
	vector<int> get_mvc_vector(void);
	
	void initialize_weight_dscore_stamp(void);
	
	void remove_vertex_from_vc(Estack<int>& uncoverd_edges, int v, Eheap<NodeWType>* vertex_heap = nullptr);
	void add_vertex_to_vc(Estack<int>& uncoverd_edges, int v, Eheap<NodeWType>* vertex_heap = nullptr);
	
	int max_dscore_coverd_vertex(void);
	int earliest_max_dscore_coverd_vertex(int tabu_v);
	int luckly_uncoverd_vertex(Estack<int> uncoverd_edges);
	
	void updateWeight(Estack<int>& uncoverd_edges);
	void forget_edge_weights(void);

public:
	MvcSolver(const Graph& _graph):
		graph( _graph ),
		v_num( _graph.v_num ), e_num( _graph.e_num ) { };

	~MvcSolver() {};

	vector<int> solveMvc(int _cutoff_time,
						 int _optimal_size,
						 double threshold_scale=0.5,
						 int seed=0) {
		cutoff_time = _cutoff_time;
		optimal_size = _optimal_size;
		threshold = v_num * threshold_scale;
		is_coverd.resize(v_num+1);
		srand(seed);
		return calculateMVC();
	};
};

#endif

