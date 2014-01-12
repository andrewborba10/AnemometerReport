//============================================================================
// AssumedEnvironment.h Jun 28, 2011
//============================================================================
#ifndef ASSUMEDENVIRONMENT_H_
#define ASSUMEDENVIRONMENT_H_
#define u1 unsigned char
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "Token.h"
#include "Utilities.h"
using namespace std;
//============================================================================
class AssumedEnvironment {
public:
	map<string, string> envparams;
	map<string, string> userinput;
	string 	sitename, description;
	float 	latitude, longitude;
	int 	altitude, towerheight;
	float 	sweptarea;
	float 	efficiency;
	float 	electricityvalue;
	float	cutinspeed, cutoutspeed;
	float 	survivalspeed;
	float 	seasonaladj;
	float 	maxgenpwr;
	float 	subsidy;

	AssumedEnvironment();
	bool ProcessFile(string environmentFileName);
	bool ValidateAndCopy();
};
//============================================================================
#endif /* ASSUMEDENVIRONMENT_H_ */
