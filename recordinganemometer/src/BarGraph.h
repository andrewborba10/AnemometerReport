//============================================================================
// BarGraph.h  Aug 12, 2011
//============================================================================
#ifndef BARGRAPH_H_
#define BARGRAPH_H_
//============================================================================
#include <iostream>
#include <map>
using namespace std;
#include "Graph.h"
#include "ReportObject.h"
#include "hpdf.h"
//============================================================================
class BarGraph : public Graph {
public:
	BarGraph(ReportPage*);
	BarGraph(Graph);
private:
	map<int, float> data;
public:
	void push(float, float);
	void draw(HPDF_REAL x, HPDF_REAL y, GraphStyleEnum style);
	void drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, GraphStyleEnum style);
};
#endif /* LINEGRAPH_H_ */
