//============================================================================
// LineGraph.cpp  Aug 12, 2011
//============================================================================
#include "LineGraph.h"
//============================================================================
LineGraph::LineGraph(ReportPage *rp) {
	this->setPage(rp);
}
//============================================================================
LineGraph::LineGraph(Graph graph) {
	this->copy(graph);
}
//============================================================================
void LineGraph::push(float x, float y) { // Overwrites old values.
	if (x < getHorzStart() || x > getHorzEnd() || y < getVertStart() || y > getVertEnd()) {
		cout << "Invalid data point (" << x << ", " << y << ")" << " in line graph titled \"" << getTitle() << "\"" << endl;
		return;
	}
	data[x] = y;
}
//============================================================================
void LineGraph::draw(HPDF_REAL x, HPDF_REAL y, GraphStyleEnum style) {
	Graph::draw(x, y, style);

	// Define colors by style.
	HPDF_REAL lineColor[3];

	switch(style) {
	case defaultGraph:
		lineColor[0] = 0.0; lineColor[1] = 0.0; lineColor[2] = 0.5;
		break;
	default:
		cout << "Error: Undefined graph style." << endl;
		lineColor[0] = 0.0; lineColor[1] = 0.0; lineColor[2] = 0.0;
		break;
	}

	HPDF_Page_SetLineWidth(getPage()->pageHandle, 1.0);
	HPDF_Page_SetRGBStroke (getPage()->pageHandle, lineColor[0], lineColor[1], lineColor[2]);
	HPDF_Page_MoveTo(getPage()->pageHandle, getXOrigin(), getYOrigin());
	map<float, float>::iterator it;
	for (it = data.begin(); it != data.end(); it++) {
		float xPoint = getXOrigin() + it->first*getHorzScale();
		float yPoint = getYOrigin() + it->second*getVertScale();

		HPDF_Page_LineTo(getPage()->pageHandle, xPoint, yPoint);
	}
	HPDF_Page_Stroke(getPage()->pageHandle);
}
//============================================================================
void LineGraph::drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, GraphStyleEnum style) {
	int width = getWidth();
	LineGraph::draw(((getPage()->width-width) / 2) + centerOffset, y, style);
}
//============================================================================
