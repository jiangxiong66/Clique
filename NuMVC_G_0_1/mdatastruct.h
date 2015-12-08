#ifndef MDATA_STRUCT_H
#define MDATA_STRUCT_H

#include <vector>
#include <set>
#include <algorithm>

using std::swap;
using std::vector;
using std::cout;
using std::endl;
using std::set;

template <typename T>
class Elist {
private:
	set<T> arr;
public:
	void add(T element) {
		arr.insert(element);
	}

	void erase(T element) {
		arr.erase(element);
	}

	void clear(void) {
		arr.clear();
	}

	bool empty(void) {
		return arr.empty();
	}
};

template <typename T>
class Estack {
private:
	vector<T> arr;
	vector<int> id_to_index;
	vector<int> index_to_id;
	int tp, msize;
public:
	Estack(void) {}

	Estack(int _msize): 
		tp(0), msize(_msize), 
		arr(vector<T>(_msize)), 
		index_to_id(vector<int>(_msize, _msize)),
		id_to_index(vector<int>(_msize, _msize)) {

	}

	T operator [](int idx) const {
		return arr[idx];
	}

	T at(int idx) {
		if(idx<0 || idx>=tp) idx = 0;
		return arr[idx];
	}

	void clear(void) {
		tp = 0;
		index_to_id = vector<int>(msize, msize);
		id_to_index = vector<int>(msize, msize);
	}

	bool empty(void) {
		return tp==0;
	}

	void pop(void) {
		if(tp) {
			tp--;
			int id = index_to_id[tp];
			id_to_index[id] = msize;
			index_to_id[tp] = msize;
		};
	}

	T top(void) {
		if(tp) {
			return arr[tp-1];
		}
	}

	void push(T _element, int id) {
		if(tp < msize && id_to_index[id]==msize) {
			id_to_index[id] = tp;
			index_to_id[tp] = id;
			arr[tp++] = _element;
		}
	}

	void swap_by_id(unsigned int aid, unsigned int bid) {
		int aindex = id_to_index[aid];
		int bindex = id_to_index[bid];
		swap(arr[aindex], arr[bindex]);
		swap(index_to_id[aindex], index_to_id[bindex]);
		swap(id_to_index[aid], id_to_index[bid]);
	}

	void remove(unsigned int id) {
		int tid = index_to_id[tp-1];
		if(id >= msize || id_to_index[id] >= msize) return ;
		swap_by_id(id, tid);
		pop();
	}

	int size(void) {
		return tp;
	}

	void output(void) {
		cout << "stack size : " << msize << endl;
		for(int i=0; i<tp; i++) {
			int idx = i;
			int id = index_to_id[idx];
			int cidx = id_to_index[id];
			int cid = index_to_id[cidx];
			cout << "{idx: " << idx <<" cidx: " << cidx <<  " id: " << id << " cid: " << cid << " " << arr[i] << "} ";
			cout << endl;
		}
		cout << endl << endl;
	}
};

template <typename T>
class Eheap {
	typedef bool (*cmpfn)(const T& lhs, const T& rhs);
private:
	bool (*pred)(const T& lhs, const T& rhs);
	vector<T> *weight;
	vector<int> id_to_heap_idx;
	vector<int> heap;
	int last;
	static const int init_size = 32;

	int parent(int idx) {
		return idx>>1;
	}

	int leftChild(int idx) {
		return idx<<1;
	}

	int rightChild(int idx) {
		return (idx<<1)|1;
	}

	void swap_by_index(int aidx, int bidx) {
		int aid = heap[aidx];
		int bid = heap[bidx];
		swap(id_to_heap_idx[aid], id_to_heap_idx[bid]);
		swap(heap[aidx], heap[bidx]);
	}

	bool compare_by_id(int aid, int bid) {
		bool flag;
		if(weight != nullptr) {
			flag = pred((*weight)[aid], (*weight)[bid]);
		} else {
			flag = pred(aid, bid);
		}
		return flag;
	}

	void floatUp(int idx) {
		int p = idx;
		while(p!=1) {
			int r = parent(p);
			int sid = heap[p];
			int rid = heap[r];
			bool flag = compare_by_id(sid, rid);
			if(!flag) break;
			swap_by_index(p, r);
			p = r;
		}
	}

	void diveDown(int idx) {
		int r = idx;

		while(true) {
			int ls = leftChild(r);
			int rs = rightChild(r);
			int nr = r;

			if(ls<=last && compare_by_id(heap[ls], heap[nr])) {
				nr = ls;
			}
			if(rs<=last && compare_by_id(heap[rs], heap[nr])) {
				nr = rs;
			}
			if(nr==r) break;
			swap_by_index(r, nr);
			r = nr;
		}
	}

public:
	/*
	Eheap(void): 
		heap(vector<int>(init_size)), 
		last(0), 
		weight(nullptr),
		pred([](const T& lhs, const T& rhs)->bool{return lhs < rhs;}) {};
	*/
	Eheap(cmpfn fn = [](const T& lhs, const T& rhs)->bool{return lhs < rhs;}): 
		heap(vector<int>(init_size)), 
		last(0), 
		weight(nullptr),
		pred(fn) {};

	Eheap(vector<T>* _weight, bool (*cmp)(const T& lhs, const T& rhs)): 
		heap(vector<int>(init_size)), 
		weight(_weight), 
		last(0),
		pred(cmp) {};

	bool empty(void) {
		return last==0;
	}

	void insert(int id) {
		last++;
		if(last >= heap.size()) heap.resize(last*2);
		if(id >= id_to_heap_idx.size()) id_to_heap_idx.resize(id+1);
		
		heap[last] = id;
		id_to_heap_idx[id] = last;

		floatUp(last);
	}

	int top(void) {
		return heap[1];
	}

	void pop(void) {
		if(empty()) return ;
		int fid = heap[1];
		swap_by_index(last--, 1);
		diveDown(1);
		id_to_heap_idx[fid] = 0;
	}

	void remove(int id) {
		int idx = id_to_heap_idx[id];
		if(!id || !id_to_heap_idx[id]) return;
		swap_by_index(last--, idx);
		if(idx>1 && compare_by_id(heap[idx], heap[parent(idx)])) {
			floatUp(idx);
		} else {
			diveDown(idx);
		}
		id_to_heap_idx[id] = 0;
	}

	void output(void) {
		cout << "last -> " << last << endl;
		if(weight) {
			cout << "weight: ";
			for(int i=0; i<weight->size(); i++) cout << "{" << i << " : " << (*weight)[i] << "} ";
			cout << endl;
		}
		
		cout << "id to heap idx:" << endl;
		for(int i=0; i<id_to_heap_idx.size(); i++) cout << "{" << i << "->" << id_to_heap_idx[i] << "} ";
		cout << endl;

		cout << "heap : " << endl;
		int cnt = 1;
		for(int i=1; i<=last && i<heap.size(); i++) {
			cout << heap[i] << " ";
			if(cnt==i) {
				cout << endl;
				cnt = cnt*2 + 1;
			}
		}
		cout << endl;
		cout << "-----------------------------" << endl;
	}
};

#endif
