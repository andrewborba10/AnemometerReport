// SDRecord.h Jun 27, 2011
//============================================================================
#ifndef SDRECORD_H_
#define SDRECORD_H_
#define u1 unsigned char
#define u2 unsigned short
#define UNDEFINED_WINDSPEED 255
//============================================================================
class SDRecord {
public:
	SDRecord();
	u2 day;//Jan 1, 2010 = 0
	u1 peakSpeed;//Instantaneous, not avg
	u1 windSpeed[240];//Each entry represents avg 6 min, 255 means undefined
};
//============================================================================
#endif /* SDRECORD_H_ */
