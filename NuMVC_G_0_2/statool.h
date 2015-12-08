#ifndef _STATOOL_H
#define _STATOOL_H

#include <chrono>
#include <map>
#include <sstream>
#include <vector>

using namespace std::chrono;
using std::string;
using std::map;
using std::vector;
using std::stringstream;

typedef steady_clock::time_point TPoint;

class RunMeasurer {
	map<string, TPoint> clocks;
	map<string, double> time_note;

public:
	RunMeasurer() { };
	RunMeasurer(vector<string>& name_list): clocks(), time_note() {
		TPoint s = steady_clock::now();
		for( auto name : name_list ) {
			time_note[name] = 0.0;
			clocks[name] = s;
		}
	};
	
	void BEGINTIME(const string& fun_name) {
		clocks[fun_name] = steady_clock::now();
	};
	
	void ENDTIME(const string& fun_name) {
		TPoint t = steady_clock::now();
		time_note[fun_name] += duration_cast<duration<double>>(t-clocks[fun_name]).count();
	};
	
	std::string getStatistic(void) const{
		stringstream stream;
		stream << "RunMeasurer : \n";
		for(auto it = time_note.begin(); it != time_note.end(); ++it) {
			stream << it->first << " : " << it->second << "s" << std::endl;
		}
		return stream.str();
	};
};

#endif

