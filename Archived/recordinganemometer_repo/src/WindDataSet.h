//============================================================================
//WindDataSet.h Jun 25, 2011
//============================================================================
#ifndef WINDDATASET_H_
#define WINDDATASET_H_
#define u2 unsigned short
#define u1 unsigned char
#define TIME_OFFSET 1262332800 // Amount of seconds between Jan 1st 1970 and Jan 1st 2010.
//===============================================
#include <cstdio>
#include <math.h>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "SDRecord.h"
#include "RecordDay.h"
#include "Token.h"
using namespace std;
//===============================================
float windSpeedToPower(int);
//===============================================
class WindDataSet {
public:
	WindDataSet();
	bool readSampleData(string fileName);
	void preprocess();

	struct tm currentDateInfo;
	struct tm startDateInfo;
	struct tm endDateInfo;
	short samplePeriod; // In days

	int absPeakSpeed;
	int absPeakAvgSpeed;
	int absPeak6MinSpeed;
	int absPeakPower;
	int absPeakAvgPower;
	int absPeak6MinPower;

	// Summary data.
	float avgEnergy;
	float avgEnergyCutIn;
	float avgEnergyCutInOut;
	float avgEnergyEff; // Considering turbine efficiency.
	float avgEnergyDay; // Considering swept area.
	float avgEnergyMonth; // Per month

	// Power data.
	vector<float> wattRanges;
	bool exceededMax;
	vector<int> dailyHours;
	vector<int> dailyMinutes;

	// Capacity data.
	float capacity;

	// Idle time data.
	int idleFor5Days;
	int idleFor3Days;
	int idleFor2Days;
	int idleFor1Day;

	int kwhFrom0to5Days;
	int kwhFrom6to10Days;
	int kwhFrom11to15Days;
	int kwhFrom16to20Days;

	// Best days data
	struct tm bestDayDateInfo[10];
	float bestDayEnergy[10];
	int numBestDays;

	// Value of electricity data.
	u2 numMonths;
	string *monthNames;
	float *monthValues;

	// Overall rating
	float rating;
private:
	list <RecordDay> r;
};
//===============================================
#endif /* WINDDATASET_H_ */
