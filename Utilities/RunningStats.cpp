#include "RunningStats.h"

// Running mean function:  (newDP - curMean)/newNumSamples + curMean
// Running sample variance function: (newDP - curMean)*(newDP - newMean)
void RunningStats::AddDataPoint(float dp) {
	numSamples++;
	if (numSamples <= 1) {
		runningMean = dp;
		runningSVariance = 0.0;
	} else {
		float oldMean = runningMean; // Need both the new and old runningMean values to calculate running SVariance.
		runningMean += (dp - runningMean)/numSamples;
		runningSVariance += (dp - oldMean)*(dp - runningMean);
	}
}

float RunningStats::GetMean() {
	return runningMean;
}

// Variance = Sample Variance / numSamples - 1
float RunningStats::GetVariance() {
	return (numSamples > 1) ? runningSVariance/(float)(numSamples-1) : 0.0;
}

void RunningStats::Reset() {
	runningMean = runningSVariance = 0.0;
	numSamples = 0;
}
