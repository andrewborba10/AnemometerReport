//============================================================================
// RecordingAnemometer.cpp
//============================================================================
#include <iostream>
#include <string>
#include "WindDataSet.h"
#include "AssumedEnvironment.h"
#include "WindReport.h"
using namespace std;
//============================================================================
//Globals
WindDataSet wds;
WindReport wr;
float windPower[255];//Per square meter, where ix is m/s*10
AssumedEnvironment assumedEnvironment;
//============================================================================
//The command line should name the files to include in the report,
//with the last filename being the name of the pdf to produce
//Example: RecordingAnemometer aDataFile moreData theOutput.pdf
int main(int argc, char* argv[]) {
	for (int i = 0; i < 255; i++){
		float ws = (float)i / 10.0f;
		windPower[i] = 1.19f / 2.0f * ws * ws * ws;//Power per square meter at the speed
	}
	//Read in the files specified on commandline
	if (!assumedEnvironment.ProcessFile(argv[1])) return 0;
	if (!assumedEnvironment.ValidateAndCopy()) return 0;
	for (int i = 2; i < argc - 1; i++){
		wds.readSampleData(argv[i]); // Computes most of data needed for report
	}
	wds.preprocess(); // Merges records that have data for the same day.
	char *saveFileName = argv[argc - 1];
	if (!wr.makeReport(wds, saveFileName)) //Computes and writes pdf
		return 0;
	return 0;
}
//============================================================================

