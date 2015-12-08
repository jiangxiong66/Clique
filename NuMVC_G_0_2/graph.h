#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>

using std::vector;

struct Edge {
	int from, to, weight;
	Edge(int _from, int _to): from(_from), to(_to), weight(1) {
	};

	Edge(int _from, int _to, int _weight): from(_from), to(_to), weight(_weight) {
	};

	void output(void);
};

struct Node {
	vector<int> neighbor_nodes;
	vector<int> neighbor_edges;
};

struct Graph {
	vector<Edge> edges;
	vector<Node> nodes;
	int v_num, e_num;

	Graph(void) { };
	Graph(int node_num): nodes(vector<Node>(node_num+1)), v_num(node_num), e_num(0) {};

	void add_edge(Edge e);
	bool isCoveredBy(vector<int>& vertexs);
	void output(void);
};

Graph readMVCGraph(const char *filename) ;

#endif

