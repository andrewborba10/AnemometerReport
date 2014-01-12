//============================================================================
// Graph.h Aug 8, 2011
//============================================================================
#ifndef GRAPH_H_
#define GRAPH_H_
//============================================================================
#include "hpdf.h"
#include "ReportObject.h"
#include "Utilities.h"
// #include "WindReport.h" // This is included at the bottom of this file.
//============================================================================
class ReportPage;
//============================================================================
class Graph {
public:
	Graph() {}
	Graph(ReportPage*);
private:
	ReportPage *page;
	float vertStart, vertEnd;
	float vertJump, vertScale; // Jump is the difference in units from one tick to the next.
	float horzStart, horzEnd;
	float horzJump, horzScale; // Scale is the number of pixel units between each jump.
	string title;
	string vertLabel;
	string horzLabel;

	float xOrigin, yOrigin; // Calculated when graph is drawn.
public:
	void clear();
	void copy(Graph);

	// Getters
	ReportPage* getPage();
	string getTitle();
	string getVertLabel();
	string getHorzLabel();
	float getVertStart();
	float getHorzStart();
	float getVertEnd();
	float getHorzEnd();
	float getVertJump();
	float getHorzJump();
	float getVertScale();
	float getHorzScale();

	const float getXOrigin();
	const float getYOrigin();
	const float getXAxisLength();
	const float getYAxisLength();
	const float getWidth();
	const float getHeight();

	// Setters
	void setPage(ReportPage*);
	void setTitle(string);
	void setVertLabel(string);
	void setHorzLabel(string);
	void setVertStart(float);
	void setHorzStart(float);
	void setVertEnd(float);
	void setHorzEnd(float);
	void setVertJump(float);
	void setHorzJump(float);
	void setVertScale(float);
	void setHorzScale(float);

	// Do-ers
	void draw(HPDF_REAL x, HPDF_REAL y, GraphStyleEnum style);
	void drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, GraphStyleEnum style);
};

#include "WindReport.h"

#endif /* GRAPH_H_ */


