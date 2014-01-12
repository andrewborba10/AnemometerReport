//============================================================================
// RecordDay.cpp Jun 25, 2011
//============================================================================
#include "RecordDay.h"
//=========================================================
RecordDay::RecordDay(SDRecord rawRec) :
	day(rawRec.day), startValidIx(240), peakSpeed(rawRec.peakSpeed) {
	for (int i = 0; i < 255; i++) minutesAtWindspeed[i] = 0;
	for (int i = 0; i < 240; i++){
		windSpeed[i] = rawRec.windSpeed[i];
		if (windSpeed[i] != UNDEFINED_WINDSPEED){
			if (startValidIx == 240) startValidIx = i;
			endValidIx = i;
		}
		// note: if there are undefined values in the middle of the array, this will still be
		// "complete"
	}
	if (startValidIx == 0 && endValidIx == 239) complete = true;
	else complete = false;
	preprocess();
}
//=========================================================
void RecordDay::preprocess() {
	//Compute avg power for the day
	avgSpeed = 0;
	float total = 0, belowCutIn = 0, aboveCutOut = 0;
	for (int i = startValidIx; i <= endValidIx; i++){
		if (windSpeed[i]==UNDEFINED_WINDSPEED) continue; // Undefined speed.
		avgSpeed += windSpeed[i];
		float powerElement = windPower[windSpeed[i]];
		total += powerElement;
		if ((float)windSpeed[i]/10.0f < assumedEnvironment.cutinspeed) belowCutIn += powerElement;
		if ((float)windSpeed[i]/10.0f > assumedEnvironment.cutoutspeed) aboveCutOut += powerElement;
		minutesAtWindspeed[windSpeed[i]] += 6;
	}
	avgSpeed /= (float)(endValidIx + 1 - startValidIx);

	avgPowerRaw = total / (float)(endValidIx + 1 - startValidIx);
	avgPowerRawK = avgPowerRaw / 1000.0;
	avgPower = avgPowerRawK * assumedEnvironment.sweptarea * assumedEnvironment.efficiency;
	avgPowerK = avgPower / 1000.0;

	//totalEnergy
	totalEnergyRaw = total / 10.0f;//Since each entry represents 6 minutes
	totalEnergyRawK = totalEnergyRaw / 1000.0;
	totalEnergy = totalEnergyRaw * assumedEnvironment.sweptarea * assumedEnvironment.efficiency;
	totalEnergyK = totalEnergy / 1000.0;

	//Below cutin energy
	belowCutInEnergyRaw = belowCutIn / 10.0f;
	belowCutInEnergyRawK = belowCutInEnergyRaw / 1000.0;

	//Above generator capacity
	aboveCutOutEnergyRaw = aboveCutOut / 10.0f;
	aboveCutOutEnergyRawK = aboveCutOutEnergyRaw / 1000.0;
}
//=========================================================
void RecordDay::merge(RecordDay rd){//Moves data from rd into this
	for (int i=rd.startValidIx; i <= rd.endValidIx; i++){
		windSpeed[i] = rd.windSpeed[i];
	}
	bool adjacent = (endValidIx == rd.startValidIx-1);
	if (rd.startValidIx < startValidIx) startValidIx = rd.startValidIx;
	if (rd.endValidIx > endValidIx) endValidIx = rd.endValidIx;
	if (startValidIx == 0 && endValidIx == 239 && adjacent) complete = true;
}
//=========================================================
