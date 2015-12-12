#include <iostream>
#include <algorithm>
#include <string>

#include "mdatastruct.h"

using namespace std;

void func(EContainer* heap) {
	while(!heap->empty()) {
		int id = heap->top();
		cout << "id : " << id << endl;
		heap->remove(id);
	}
}

int main() {
	vector<double> weight{1, 2, 3, 4, 5, 6, 7, 8};
	Estack<double> heap(weight, [](const double& lhs, const double& rhs){return lhs < rhs;});
	string cmd;
	int key;
	int id;
	
	while(cout << ":" && cin>>cmd) {
		if(cmd == "top") {
			cout << "top : " << heap.top() << endl;
			cout << "weight : " << weight[id] << endl;
		} else if (cmd == "insert") {
			cin >> id;
			heap.insert(id);
		} else if (cmd == "remove") {
			cin >> id;
			heap.remove(id);
		} else if (cmd == "find") {
			cin >> id;
			cout << heap.find(id) << endl;
		} else if (cmd == "func") {
			func(&heap);
		}
		cout << heap.toString() << endl;
	}
	return 0;
}
