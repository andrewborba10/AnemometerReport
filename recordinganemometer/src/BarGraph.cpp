//============================================================================
// BarGraph.cpp  Aug 12, 2011
//============================================================================
#include "BarGraph.h"
//============================================================================
BarGraph::BarGraph(ReportPage *rp) {
	this->setPage(rp);
}
//============================================================================
BarGraph::BarGraph(Graph graph) {
	this->copy(graph);
}
//============================================================================
void BarGraph::push(float x, float y) { // Overwrites old values.
	if (x < getHorzStart() || x > getHorzEnd() || y < getVertStart() || y > getVertEnd()) {
		cout << "Invalid data point (" << x << ", " << y << ")" << " in bar graph titled \"" << getTitle() << "\"" << endl;
		return;
	}
	data[x] = y;
}
//============================================================================
void BarGraph::draw(HPDF_REAL x, HPDF_REAL y, GraphStyleEnum style) {
	Graph::draw(x, y, style);

	// Define colors by style.
	float barWidthRatio;
	HPDF_REAL barColor[3];

	switch(style) {
	case defaultGraph:
		barWidthRatio = 0.5;
		barColor[0] = 0.0; barColor[1] = 0.0; barColor[2] = 0.5;
		break;
	default:
		cout << "Error: Undefined graph style." << endl;
		barWidthRatio = 1.0;
		barColor[0] = 0.0; barColor[1] = 0.0; barColor[2] = 0.0;
		break;
	}

	HPDF_Page_SetLineWidth(getPage()->pageHandle, 1.0);

	map<int, float>::iterator it;
	for (it = data.begin(); it != data.end(); it++) {
		float xPoint = getXOrigin() + it->first*getHorzScale();

		HPDF_Page_SetRGBStroke (getPage()->pageHandle, 0.0, 0.0, 0.0);
		DrawBox(getPage()->pageHandle, 1.0, xPoint + (getHorzJump()*getHorzScale() * (1 - barWidthRatio))/2, getYOrigin(), getHorzJump()*getHorzScale()*barWidthRatio, it->second*getVertScale(), BOX_OUTLINE_ALL);

		HPDF_Page_SetRGBStroke (getPage()->pageHandle, barColor[0], barColor[1], barColor[2]);
		DrawBoxFill(getPage()->pageHandle, xPoint + (getHorzJump()*getHorzScale() * (1 - barWidthRatio))/2, getYOrigin(), getHorzJump()*getHorzScale()*barWidthRatio, it->second*getVertScale());
	}
}
//============================================================================
void BarGraph::drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, GraphStyleEnum style) {
	int width = getWidth();
	BarGraph::draw(((getPage()->width-width) / 2) + centerOffset, y, style);
}
//============================================================================
