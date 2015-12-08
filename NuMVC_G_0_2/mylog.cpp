#include "mylog.h"

#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::ofstream;

static ofstream log_os;

void initLog(const char *filename) {
	if(log_os.is_open()) {
		log_os.close();
	}
	log_os.open(filename);
}

void endLog(void) {
	if(log_os.is_open()) {
		log_os.close();
	}
}

void Log(stringstream& stream) {
	log_os << stream.str();
	cout << stream.str();
}

void Log(const string& str) {
	log_os << str;
	cout << str;
}

