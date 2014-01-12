//============================================================================
// LineGraph.h  Aug 12, 2011
//============================================================================
#ifndef LINEGRAPH_H_
#define LINEGRAPH_H_
//============================================================================
#include <iostream>
#include <map>
using namespace std;
#include "Graph.h"
#include "ReportObject.h"
#include "hpdf.h"
//============================================================================
class LineGraph : public Graph {
public:
	LineGraph(ReportPage*);
	LineGraph(Graph);
private:
	map<float, float> data;
public:
	void push(float, float);
	void draw(HPDF_REAL x, HPDF_REAL y, GraphStyleEnum style);
	void drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, GraphStyleEnum style);
};
#endif /* LINEGRAPH_H_ */
