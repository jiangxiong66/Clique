#include "graph.h"

#include <vector>
#include <fstream>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;

Graph readMVCGraph(const char *filename) {
	ifstream input;
	input.open(filename);
	if(!input.is_open()) {
		return Graph(0);
	}

	string str;
	int node_num, edge_num;
	input >> str >> str >> node_num >> edge_num;

	Graph graph(node_num);

	for(int i=0; i<edge_num; i++) {
		int from, to;
		input >> str >> from >> to;
		graph.add_edge( Edge(from, to) );
	}

	return graph;
}

void Edge::output(void) {
	cout << "{" << from << "," << to << "}";
}

void Graph::add_edge(Edge e) {
	edges.push_back(e);
	e_num++;

	int edge_id = edges.size()-1;
	int u = e.from;
	int v = e.to;
	nodes[u].neighbor_nodes.push_back(v);
	nodes[v].neighbor_nodes.push_back(u);
	nodes[u].neighbor_edges.push_back(edge_id);
	nodes[v].neighbor_edges.push_back(edge_id);
}

bool Graph::isCoveredBy(vector<int>& vertexs) {
	vector<bool> edge_check(edges.size(), false);
	for( auto v_id : vertexs ) {
		if(v_id>=0 && v_id<nodes.size()) {
			for( auto e_id : nodes[v_id].neighbor_edges ) {
				edge_check[e_id] = true;
			}
		}
	}

	for( auto flag : edge_check ) {
		if(!flag) return false;
	}
	return true;
}

void Graph::output(void) {
	for(int i=0; i<nodes.size(); i++) {
		cout << "#" << i << " Degree = " << nodes[i].neighbor_nodes.size() << endl;
		cout << "neighbor v: ";
		for(auto v : nodes[i].neighbor_nodes) {
			cout<< v << " ";
		}
		cout << endl;
		cout << "neighbor e: ";
		for(auto e : nodes[i].neighbor_edges) {
			edges[e].output();
			cout << " ";
		}
		cout << endl << endl;
	}
}

