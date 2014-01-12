//============================================================================
// WindDataSet.cpp Jun 25, 2011
//============================================================================
#include "WindDataSet.h"
//============================================================================
bool compareRecordDay(RecordDay first, RecordDay second) {
	if (first.day < second.day) return true;
	if ((first.day == second.day) && (first.startValidIx < second.startValidIx)) return true;
	return false;
}

bool compareRecordDay2(RecordDay first, RecordDay second) {
	if (first.totalEnergy > second.totalEnergy) return true;
	return false;
}
//============================================================================
WindDataSet::WindDataSet() {
}
//============================================================================
bool WindDataSet::readSampleData(string fileName) {
	//Here I read in some fake data from a csv file
	FILE* fakeFile = fopen((fileName + ".csv").c_str(), "r");
	FILE* fsim = fopen(fileName.c_str(), "w");
	u2 numRecs;
	int day, peakSpeed, windSpeed;
	fscanf(fakeFile, "%hu", &numRecs);
	fwrite(&numRecs, 2, 1, fsim);
	SDRecord sd;
	for (int i = 0; i < numRecs; i++){
		fscanf(fakeFile, "%i", &day);
		fscanf(fakeFile, "%i", &peakSpeed);
		sd.day = (u1) day;
		sd.peakSpeed = (u1) peakSpeed;
		for (int j = 0; j < 240; j++){
			fscanf(fakeFile, "%i", &windSpeed);
			sd.windSpeed[j] = (u1) windSpeed;
		}
		fwrite(&sd, sizeof(SDRecord), 1, fsim);
	}
	fclose(fakeFile);
	fclose(fsim);

	//Now read in the file as I would expect it from the sd card
	ifstream f(fileName.c_str(), ios_base::in | ios_base::binary);
	if (f.fail()) {
		cout << "Couldn't read wind data file!" << endl;
		return false;
	}
	short *numDays = new short;
	SDRecord *rawRec = new SDRecord;
	f.seekg(0, ios_base::beg);
	f.read((char*)numDays, 2);
	for (int i = 0; i < *numDays; i++){
		f.read((char*)rawRec, sizeof(SDRecord));
		RecordDay rec = RecordDay(*rawRec);//Constructor does the preprocess
		r.push_back(rec);
	}
	delete numDays;
	delete rawRec;
	f.close();
	return true;
}
//============================================================================
void WindDataSet::preprocess() { // Finds and merges records with data from the same day.
	r.sort(compareRecordDay);
	//There might be adjacent elements of the same day, so merge these
	list<RecordDay>::iterator it;
	RecordDay first, second;
	for (it = r.begin();;){
		first = *it;
		if (++it == r.end()) break;
		second = *it;
		if (first.day == second.day){
			first.merge(second);
			r.erase(it--);
		}
	}

	// Get times used in the report.
	time_t currentDate = time(NULL);
	it = r.begin();
	time_t startDate = it->day * 60 * 60 * 24 + TIME_OFFSET;
	it = r.end();
	it--;
	time_t endDate = it->day * 60 * 60 * 24 + TIME_OFFSET;

	currentDateInfo = *localtime(&currentDate);
	startDateInfo = *localtime(&startDate);
	endDateInfo = *localtime(&endDate);

	char strCurrentDate[64], strStartDate[64], strEndDate[64];
	strftime(strCurrentDate, 64, "%b %d, %Y", &currentDateInfo); // %A is full week day name.
	strftime(strStartDate, 64, "%b %d, %Y", &startDateInfo);
	strftime(strEndDate, 64, "%b %d, %Y", &endDateInfo);

	// Get the number of days that we have wind data for.
	samplePeriod = r.size();

	absPeakSpeed = 0; // Find peak speed among all RecordDays.
	absPeakAvgSpeed = 0; // Might not need this.
	absPeak6MinSpeed = 0;

	for (it = r.begin(); it != r.end(); it++) {
		if (it->peakSpeed > absPeakSpeed) absPeakSpeed = it->peakSpeed;
		if (it->avgSpeed > absPeak6MinSpeed) absPeak6MinSpeed = it->avgSpeed;
		for (int i = 0; i < 240; i++) {
			if (it->windSpeed[i] == UNDEFINED_WINDSPEED) continue;
			if (it->windSpeed[i] > absPeak6MinSpeed) absPeak6MinSpeed = it->windSpeed[i];
		}
	}

	absPeakPower = windSpeedToPower(absPeakSpeed);
	absPeakAvgPower = windSpeedToPower(absPeakAvgSpeed); // Might not need this.
	absPeak6MinPower = windSpeedToPower(absPeak6MinSpeed);

	// Summary data.
	float totalEnergy = 0;	// Calculate average energy production
	float totalEnergyCutIn = 0;
	float totalEnergyCutInOut = 0;
	for (it = r.begin(); it != r.end(); it++) {
		totalEnergy += it->totalEnergyRawK;
		totalEnergyCutIn += it->totalEnergyRawK - it->belowCutInEnergyRawK;
		totalEnergyCutInOut += it->totalEnergyRawK - it->belowCutInEnergyRawK - it->aboveCutOutEnergyRawK;
	}
	avgEnergy = totalEnergy / (float)r.size();
	avgEnergyCutIn = totalEnergyCutIn / (float)r.size();
	avgEnergyCutInOut = totalEnergyCutInOut / (float)r.size();
	avgEnergyEff = avgEnergyCutInOut * assumedEnvironment.efficiency;
	avgEnergyDay = avgEnergyEff * assumedEnvironment.sweptarea;
	avgEnergyMonth = avgEnergyDay * 30;

	// Power data.
	float maxGenPwrK = (float)assumedEnvironment.maxgenpwr / 1000.f;
	int peakCappedPower = (absPeak6MinPower > maxGenPwrK) ? maxGenPwrK : absPeak6MinPower;

	exceededMax = false;
	for (it = r.begin(); it != r.end(); it++) {
		if (absPeak6MinPower > maxGenPwrK) exceededMax = true;
	}

	wattRanges.push_back(0);
	wattRanges.push_back(windSpeedToPower(assumedEnvironment.cutinspeed));

	int jumpValue = ceil((float)peakCappedPower / 10.f);
	if (peakCappedPower > 1000) jumpValue = (jumpValue / 1000.f) * 1000.f;
	if (peakCappedPower > 100) jumpValue = (jumpValue / 100.f) * 100.f;
	else if (peakCappedPower > 10) jumpValue = (jumpValue / 10.f) * 10.f;
	if (jumpValue == 0) {
		cout << "Error: Jump value of 0 during power table calculations." << endl;
		return;
	}
	int cur = windSpeedToPower(assumedEnvironment.cutinspeed) + jumpValue;
	while (cur <= peakCappedPower) {
		wattRanges.push_back(cur);
		cur += jumpValue;
	}

	if (wattRanges.back() < peakCappedPower)
		wattRanges.push_back(peakCappedPower);

	if (exceededMax) {
		wattRanges.push_back(windSpeedToPower(absPeak6MinSpeed));
	}

	vector<int> numMinutes(wattRanges.size(), 0);
	int totalMinutes = 0;
	for (it = r.begin(); it != r.end(); it++) {
		for (int i = 0; i < 255; i++) {
			if (it->minutesAtWindspeed[i] == 0) continue;
			totalMinutes += it->minutesAtWindspeed[i];
			for (unsigned int j = 0; j < wattRanges.size()-1; j++) {
				if (floor(windSpeedToPower(i)) >= wattRanges[j] && floor(windSpeedToPower(i)) <= wattRanges[j+1]) {
					numMinutes[j] += it->minutesAtWindspeed[i];
					break;
				}
			}
		}
	}

	for (unsigned int i = 0; i < wattRanges.size(); i++) {
		dailyHours.push_back(numMinutes[i] / 60);
		dailyMinutes.push_back(numMinutes[i] % 60);
	}

	// Capacity data.

	// Numerator calculations
	float absTotalEnergy = 0;
	for (it = r.begin(); it != r.end(); it++) absTotalEnergy += it->totalEnergy;

	// Denominator calculations
	int totalHours = totalMinutes / 60;

	capacity = (absTotalEnergy / (float)(assumedEnvironment.maxgenpwr * totalHours)) * 100;

	// Idle time data.
	int consecutiveIdleDays = 0;
	idleFor5Days = idleFor3Days = idleFor2Days = idleFor1Day = 0;
	kwhFrom0to5Days = kwhFrom6to10Days = kwhFrom11to15Days = kwhFrom16to20Days = 0;
	for (it = r.begin(); it != r.end(); it++) {
		if (it->totalEnergyK >= 0 && it->totalEnergyK <= 5) kwhFrom0to5Days++;
		if (it->totalEnergyK >= 6 && it->totalEnergyK <= 10) kwhFrom6to10Days++;
		if (it->totalEnergyK >= 11 && it->totalEnergyK <= 15) kwhFrom11to15Days++;
		if (it->totalEnergyK >= 16 && it->totalEnergyK <= 20) kwhFrom16to20Days++;

		if (it->totalEnergy > 0) {
			consecutiveIdleDays = 0;
			continue;
		}
		consecutiveIdleDays++;
		if ((consecutiveIdleDays % 5) == 0) idleFor5Days++;
		if ((consecutiveIdleDays % 3) == 0) idleFor3Days++;
		if ((consecutiveIdleDays % 2) == 0) idleFor2Days++;
		if (consecutiveIdleDays >= 1) idleFor1Day++;
	}

	// Best days data.
	list<RecordDay> r2(r);
	r2.sort(compareRecordDay2);
	numBestDays = 0;
	time_t dayDate;
	for (it = r2.begin(); it != r2.end(); it++) {
		dayDate = it->day * 60 * 60 * 24 + TIME_OFFSET;
		bestDayDateInfo[numBestDays] = *localtime(&dayDate);
		bestDayEnergy[numBestDays] = it->totalEnergyK;
		if (++numBestDays >= 10) break;
	}

	// Value of electricity data.
	numMonths = (endDateInfo.tm_mon+1) - startDateInfo.tm_mon;
	monthNames = new string[numMonths];
	monthValues = new float[numMonths];

	char buf[16];
	struct tm dummy;
	dummy = startDateInfo;
	for (int i = 0; i < numMonths; i++) {
		strftime (buf, 16, "%B", &dummy);
		monthNames[i] = string(buf);
		dummy.tm_mon++;
	}

	// Overall rating.
	rating = 8.4;

	// Copy all of this data to tokens so the text file can reference it.
	SetTokenValue<int>("sampleperiod", samplePeriod);
	SetTokenValue<string>("currentdate1", string(strCurrentDate));
	SetTokenValue<string>("startdate1", string(strStartDate));
	SetTokenValue<string>("enddate1", string(strEndDate));
	SetTokenValue<short>("sampleperiod", samplePeriod);
	SetTokenFloat("dailyenergyavg1", avgEnergy, 1);
	SetTokenFloat("dailyenergyavg2", avgEnergyCutIn, 1);
	SetTokenFloat("dailyenergyavg3", avgEnergyCutInOut, 1);
	SetTokenFloat("dailyenergyavg4", avgEnergyEff, 1);
	SetTokenFloat("dailykwhavg", avgEnergyDay, 2);
	SetTokenFloat("monthlykwhavg", avgEnergyMonth, 2);
	SetTokenFloat("abspeakspeed", (float)absPeakSpeed / 10.0f, 1);
	SetTokenFloat("abspeak6minspeed", (float)absPeak6MinSpeed / 10.0f, 1);
	SetTokenFloat("capacity", capacity, 2);
}
//============================================================================
