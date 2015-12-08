#ifndef _MYLOG_H
#define _MYLOG_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include "statool.h"

using std::vector;
using std::string;
using std::ofstream;

void initLog(const char *filename);
void endLog(void);
void Log(stringstream& stream);
void Log(const string& str);

#endif

