#ifndef RUNNINGSTATS_H_
#define RUNNINGSTATS_H_

class RunningStats {
private:
	short numSamples;
	float runningMean;
	float runningSVariance;
public:
	RunningStats() : numSamples(0), runningMean(0.0), runningSVariance(0.0) {}
	short GetNumSamples() { return numSamples; }
	void AddDataPoint(float);
	float GetMean();
	float GetVariance();
	void Reset();
};

#endif /* RUNNINGSTATS_H_ */
