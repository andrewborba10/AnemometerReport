//============================================================================
// RecordDay.h Jun 25, 2011
//============================================================================
#ifndef RECORDDAY_H_
#define RECORDDAY_H_
#define u1 unsigned char
#define u2 unsigned short
//=========================================================
#include "SDRecord.h"
#include "AssumedEnvironment.h"
#include "Utilities.h"
using namespace std;
//=========================================================
extern AssumedEnvironment assumedEnvironment;
extern float windPower[255];
//=========================================================
class RecordDay {
public:
	short day;//Jan 1, 2010 = 0
	bool complete; // means startValidIx = 0 and endValidIx = 239
	short startValidIx;//Fully filled this is 0, undefined is 240 (size of the array)
	short endValidIx;//Fully filled, this is 239, undefined is 240
	short windSpeed[240];//Each entry represents avg 6 min, UNDEFINED_WINDSPEED means undefined
	short peakSpeed;//Instantaneous, not avg
	float avgSpeed;
	float totalPower, totalPowerK;
	float avgPower, avgPowerK;
	float totalEnergyRaw, totalEnergyRawK;
	float totalEnergyCap, totalEnergyCapK;
	float cutInEnergy, cutInEnergyK;
	float cutInOutEnergy, cutInOutEnergyK;
	float totalEnergyReal, totalEnergyRealK;
	short minutesAtWindspeed[255];

	RecordDay(){};
	RecordDay(SDRecord rawRec);
	void merge(RecordDay rd);

private:
	void preprocess();
};
//=========================================================
#endif /* RECORDDAY_H_ */
