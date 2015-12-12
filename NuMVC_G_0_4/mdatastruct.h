#ifndef MDATA_STRUCT_H
#define MDATA_STRUCT_H

#define DEBUG

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using std::vector;
using std::endl;

class EContainer{
public:
	EContainer(){};
	virtual ~EContainer(){};

	virtual int size(void) = 0;
	virtual bool empty(void) = 0;

	virtual bool find(int id) = 0;

	virtual int top(void) = 0;
	virtual void insert(int id) = 0;
	virtual void remove(int id) = 0;
	virtual void updateWeight(int id) = 0;
};

template<typename T>
class Estack: public EContainer {
	typedef bool (*cmpfn)(const T& lhs, const T& rhs);
private:
	const vector<T>& weight;
	const int range;
	cmpfn lessthan;
private:
	vector<int> stack;
	vector<int> idx_of_id;
	int tp;
	int capacity;
public:
	Estack(const vector<T>& _weight, cmpfn _lessthan):
		weight(_weight), lessthan(_lessthan), tp(0), range(_weight.size()) {
		capacity = range+1;
		stack = vector<int>(capacity, 0);
		idx_of_id = vector<int>(capacity, capacity);
	};

	int size(void) {
		return tp;
	}

	bool empty(void) {
		return size() == 0;
	}

	bool find(int id) {
		return idx_of_id[id] != capacity;
	}

	int top(void) {
		int max_id = stack[0];
		T max_weight = weight[max_id];
		for(int i=0; i<tp; i++) {
			int id = stack[i];
			if( lessthan(weight[max_id], weight[id]) ) {
				max_id = id;
				max_weight = weight[id];
			}
		}
		return max_id;
	}

	void insert(int id) {
		if(find(id)) {
			#ifdef DEBUG
			std::cout << "STACK INSERT ERROR:id = " << id << " is already in stack" << endl;
			#endif
			return ;
		}
		stack[tp] = id;
		idx_of_id[id] = tp;
		++tp;
	}

	void remove(int id) {
		if(!find(id)) {
			#ifdef DEBUG
			std::cout << "STACK REMOVE ERROR:id = " << id << " is not in stack" << endl;
			#endif
			return ;
		}
		tp--;
		int idx = idx_of_id[id];
		int tp_idx = tp;
		int tp_id = stack[tp];
		idx_of_id[tp_id] = idx;
		stack[idx] = tp_id;
		idx_of_id[id] = capacity;
	}

	void updateWeight(int id) {
		return ;
	}
	
	std::string toString(void) {
		std::stringstream os;
		os << "stack size = " << size() << " capacity = " << capacity << endl;
		os << "tp -> " << tp << endl;
		os << "id in stack: ";
		for(int i=0; i<tp; i++) os << stack[i] << " ";
		os << endl;
		for(int i=0; i<capacity; i++) os << "[" << i << "|" << idx_of_id[i] << "] ";
		os << endl;
		return os.str();
	}
};

#endif
