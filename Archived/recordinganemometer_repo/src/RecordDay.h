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
using namespace std;
//=========================================================
extern AssumedEnvironment assumedEnvironment;
extern float windPower[255];
//=========================================================
class RecordDay {
public:
	u2 day;//Jan 1, 2010 = 0
	bool complete; // means startValidIx = 0 and endValidIx = 239
	u1 startValidIx;//Fully filled this is 0, undefined is 240 (size of the array)
	u1 endValidIx;//Fully filled, this is 239, undefined is 240
	u1 windSpeed[240];//Each entry represents avg 6 min, UNDEFINED_WINDSPEED means undefined
	u1 peakSpeed;//Instantaneous, not avg
	float avgSpeed;
	float avgPowerRaw, totalEnergyRaw, belowCutInEnergyRaw, aboveCutOutEnergyRaw;
	float avgPowerRawK, totalEnergyRawK, belowCutInEnergyRawK, aboveCutOutEnergyRawK;
	float avgPower, totalEnergy;
	float avgPowerK, totalEnergyK;
	short minutesAtWindspeed[255];

	RecordDay(){};
	RecordDay(SDRecord rawRec);
	void merge(RecordDay rd);

private:
	void preprocess();
};
//=========================================================
#endif /* RECORDDAY_H_ */
