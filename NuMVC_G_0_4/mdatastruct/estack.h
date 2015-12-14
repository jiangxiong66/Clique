/*
 * File: estack.h
 *---------------
 * This file contains the private section of the estack interface.
 */

typedef bool (*cmpfn)(const T& lhs, const T& rhs);

const vector<T>& weight;
const int range;
cmpfn lessthan;

vector<int> stack;
vector<int> idx_of_id;
int tp;
int capacity;
