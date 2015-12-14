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

	virtual int size(void) const = 0;
	virtual bool empty(void) const = 0;

	virtual bool find(int id) const = 0;
	virtual int top(void) const = 0;
	
	virtual void insert(int id) = 0;
	virtual void remove(int id) = 0;
	virtual void updateWeight(int id) = 0;
};

template<typename T>
class Estack: public EContainer {
private:
	#include "estack.h"
public:
	Estack(const vector<T>& _weight, cmpfn _lessthan);
	~Estack();

	int size(void) const;
	bool empty(void) const;
	
	bool find(int id) const;
	int top(void) const;
	
	int operator [](int idx);

	void insert(int id);
	void remove(int id);

	void updateWeight(int id);
	std::string toString(void) const;

	class Iterator {
	public:
		Iterator();
		~Iterator();

		int operator *();
		Iterator& operator ++(int);
		bool operator !=(const Iterator& other) const ;
	private:
		Estack *sp;
		int index;

		Iterator(Estack* sp, int p);
		friend class Estack;
	};

	Iterator begin();
	Iterator end();
	
	friend class Iterator;
};

#include "estack.cpp"

#endif
