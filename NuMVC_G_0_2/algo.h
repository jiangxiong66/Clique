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
	vector<int> dscore;
	vector<int> edge_weight;
	vector<int> conf_change;
	vector<int> time_stamp;

private:
	static const double p_scale;
	static const int IN_TO_VC;
	static const int OUT_FROM_VC;

private:
	vector<int> is_coverd;

	vector<int> calculateMVC(void);
	void initialize_weight_dscore_stamp(void);
	void remove_vertex_from_vc(Estack<int>& uncoverd_edges, int v, Eheap<int>* vertex_heap = nullptr);
	void add_vertex_to_vc(Estack<int>& uncoverd_edges, int v, Eheap<int>* vertex_heap = nullptr);
	void move_vertex(int direction, Estack<int>& uncoverd_edges, int v, Eheap<int>* vertex_heap = nullptr);
	vector<int> getMvcGreedly(void);

public:
	MvcSolver(const Graph& _graph):
		graph( _graph ),
		v_num( _graph.v_num ), e_num( _graph.e_num ),
		dscore     ( vector<int>(_graph.v_num+1, 0) ),
		conf_change( vector<int>(_graph.v_num+1, 0) ),
		time_stamp ( vector<int>(_graph.v_num+1, 0) ),
		edge_weight( vector<int>(_graph.e_num, 0) ) { };

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

