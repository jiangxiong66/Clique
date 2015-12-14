#ifdef MDATA_STRUCT_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using std::vector;
using std::endl;

template<typename T>
Estack<T>::Estack(const vector<T>& _weight, cmpfn _lessthan):
	weight(_weight), lessthan(_lessthan), tp(0), range(_weight.size()) {
	capacity = range+1;
	stack = vector<int>(capacity, 0);
	idx_of_id = vector<int>(capacity, capacity);
}

template<typename T>
Estack<T>::~Estack() {
}

template<typename T>
inline int Estack<T>::size(void) const {
	return tp;
}

template<typename T>
inline bool Estack<T>::empty(void) const {
	return size() == 0;
}

template<typename T>
inline bool Estack<T>::find(int id) const {
	return idx_of_id[id] != capacity;
}

template<typename T>
int Estack<T>::top(void) const {
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

template<typename T>
int Estack<T>::operator [](int idx) {
	return stack[idx];
}

template<typename T>
void Estack<T>::insert(int id) {
	if(find(id)) {
		#ifdef ESTACK_DEBUG
		std::cout << "STACK INSERT ERROR:id = " << id << " is already in stack" << endl;
		#endif
		return ;
	}
	stack[tp] = id;
	idx_of_id[id] = tp;
	++tp;
}

template<typename T>
void Estack<T>::remove(int id) {
	if(!find(id)) {
		#ifdef ESTACK_DEBUG
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

template<typename T>
void Estack<T>::updateWeight(int id) {
	return ;
}

template<typename T>
std::string Estack<T>::toString(void) const {
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

/*
 *Iterator: begin() and end()
 */

template<typename T>
typename Estack<T>::Iterator Estack<T>::begin(void) {
	return Iterator(this, 0);
}

template<typename T>
typename Estack<T>::Iterator Estack<T>::end(void) {
	return Iterator(this, size());
}

/*
 *Estack<T>::Iterator implementation
 */

template<typename T>
Estack<T>::Iterator::Iterator(void) {
	sp = nullptr;
}

template<typename T>
Estack<T>::Iterator::~Iterator(void) {
}

template<typename T>
Estack<T>::Iterator::Iterator(Estack<T>* stack, int p) {
	sp = stack;
	index = p;
}

template<typename T>
typename Estack<T>::Iterator& Estack<T>::Iterator::operator ++(int) {
	index++;
	return *this;
}

template<typename T>
int Estack<T>::Iterator::operator *() {
	return (*sp)[index];
}

template<typename T>
bool Estack<T>::Iterator::operator !=(const Iterator& other) const {
	return (this->sp != other.sp || this->index != other.index);
}

#endif 
