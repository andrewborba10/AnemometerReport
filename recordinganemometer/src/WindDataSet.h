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
class WindDataSet {
public:
	WindDataSet();
	bool readSampleData(string fileName);
	void preprocess();

	struct tm currentDateInfo;
	struct tm startDateInfo;
	struct tm endDateInfo;
	short samplePeriod; // In days

	// Monthly energy totals.
	string monthNames[12];
	float monthlyEnergy[12];
	float monthlyEnergyK[12];

	// Peak instantaneous speed/power among all days.
	float absPeakSpeed, absPeakPower;

	// Find peak 6-min average speed/power among all days, any speeds above the cut-out are ignored.
	float absPeak6MinSpeed, absPeak6MinPower;

	// Progressive calculations.
	float avgEnergyDayRaw;		// Total energy from a perfect/limitless turbine.
	float avgEnergyCutIn;		// Exclude energy from low winds.
	float avgEnergyCutInOut;	// Exclude energy from high winds.
	float avgEnergyEff;			// Considering turbine efficiency.
	float avgEnergySArea;		// Considering swept area.
	float avgEnergyDay;			// Avg real-world energy per-day.
	float avgEnergyMonth;		// Avg energy per-month.

	// Power data.
	vector<float> speedRanges;
	vector<float> wattRanges; // Twice the size as speed ranges.
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
	float monthlyElecValues[12];

	// Tons of carbon saved.
	float monthlyCO2Values[12];

	// Overall rating
	float rating;
private:
	list <RecordDay> r;
};
//===============================================
#endif /* WINDDATASET_H_ */
