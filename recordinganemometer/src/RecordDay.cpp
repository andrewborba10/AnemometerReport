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
	float rawPowerTotal = 0, cutInPowerTotal = 0, cutInOutPowerTotal = 0, realPowerTotal = 0;
	for (int i = startValidIx; i <= endValidIx; i++){
		if (windSpeed[i]==UNDEFINED_WINDSPEED) continue; // Undefined speed.
		avgSpeed += windSpeed[i];
		rawPowerTotal += windPower[windSpeed[i]];
		cutInPowerTotal += wsToPow(windSpeed[i], false, false, false ,false, true, false);
		cutInOutPowerTotal += wsToPow(windSpeed[i], false, false, false ,false, true, true);
		realPowerTotal += wsToRealPow(windSpeed[i], false);
		minutesAtWindspeed[windSpeed[i]] += 6;
	}
	avgSpeed /= (float)(endValidIx + 1 - startValidIx);

	// Total power.
	totalPower = realPowerTotal;
	totalPowerK = totalPower / 1000.0;

	avgPower = realPowerTotal / (float)(endValidIx + 1 - startValidIx);
	avgPowerK = avgPower / 1000.0;

	// Energy that would have been produced with a limitless, perfectly efficient turbine.
	totalEnergyRaw = rawPowerTotal / 10.0f; //Since each entry represents 6 minutes
	totalEnergyRawK = totalEnergyRaw / 1000.0;

	// Below cut-in speed.
	cutInEnergy = cutInPowerTotal / 10.0f;
	cutInEnergyK = cutInEnergy / 1000.0;

	// Above generator capacity
	cutInOutEnergy = cutInOutPowerTotal / 10.0f;
	cutInOutEnergyK = cutInOutEnergy / 1000.0;

	// Energy that would be produced by this specific turbine.
	totalEnergyReal = realPowerTotal / 10.0f;
	totalEnergyRealK = totalEnergyReal / 1000.0;
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
