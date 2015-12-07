#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;

struct Edge {
	int from, to;
	Edge(int _from, int _to): from(_from), to(_to) {
	};

	void output(void) {
		cout << "{" << from << "," << to << "}";
	}
};

struct Node {
	vector<int> neighbor_nodes;
	vector<int> neighbor_edges;
};

struct Graph {
	vector<Edge> edges;
	vector<Node> nodes;
	int v_num, e_num;

	Graph(int node_num): nodes(vector<Node>(node_num+1)), v_num(node_num), e_num(0) {};

	void add_edge(Edge e) {
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

	bool isCoveredBy(vector<int>& vertexs) {
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

	void output(void) {
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
};

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

#endif
