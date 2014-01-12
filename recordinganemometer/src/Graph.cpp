//============================================================================
// Graph.cpp Aug 8, 2011
//============================================================================
#include "Graph.h"
#include "ReportPage.h"
//============================================================================
const int gGraphPadding = 50; // Space around the graph to allow for labels.
// These bounds formed by the padding will be used for centering.
const int gGraphExtraSpace = gGraphPadding*2;
const int gVertTickLen = 5; // Ticks on the y-axis.
const int gHorzTickLen = 5; // Ticks on the x-axis.
const int gVertTickLabelDist = 2; // Distance from right side of tick label from the tick.
const int gHorzTickLabelDist = 10; // Distance from the bottom of the tick label text from the tick.
//============================================================================
Graph::Graph(ReportPage *rp)
	:page(rp) {
}
//============================================================================
void Graph::clear() {

}
//============================================================================
void Graph::copy(Graph graph) {
	page = graph.page;
	vertStart = graph.vertStart;
	vertEnd = graph.vertEnd;
	vertJump = graph.vertJump;
	vertScale = graph.vertScale;
	horzStart = graph.horzStart;
	horzEnd = graph.horzEnd;
	horzJump = graph.horzJump;
	horzScale = graph.horzScale;
	title = graph.title;
	vertLabel = graph.vertLabel;
	horzLabel = graph.horzLabel;
}
//============================================================================
ReportPage* Graph::getPage() {
	return page;
}
//============================================================================
string Graph::getTitle() {
	return title;
}
//============================================================================
string Graph::getVertLabel() {
	return vertLabel;
}
//============================================================================
string Graph::getHorzLabel() {
	return horzLabel;
}
//============================================================================
float Graph::getVertStart() {
	return vertStart;
}
//============================================================================
float Graph::getHorzStart() {
	return horzStart;
}
//============================================================================
float Graph::getVertEnd() {
	return vertEnd;
}
//============================================================================
float Graph::getHorzEnd() {
	return horzEnd;
}
//============================================================================
float Graph::getVertJump() {
	return vertJump;
}
//============================================================================
float Graph::getHorzJump() {
	return horzJump;
}
//============================================================================
float Graph::getVertScale() {
	return vertScale;
}
//============================================================================
float Graph::getHorzScale() {
	return horzScale;
}
//============================================================================
const float Graph::getXAxisLength() {
	return (horzEnd - horzStart) * horzScale;
}
//============================================================================
const float Graph::getYAxisLength() {
	return (vertEnd - vertStart) * vertScale;
}
//============================================================================
const float Graph::getWidth() {
	return (horzEnd - horzStart) * horzScale + gGraphExtraSpace;
}
//============================================================================
const float Graph::getHeight() {
	return (vertEnd - vertStart) * vertScale + gGraphExtraSpace;
}
//============================================================================
const float Graph::getXOrigin() {
	return xOrigin;
}
//============================================================================
const float Graph::getYOrigin() {
	return yOrigin;
}
//============================================================================
void Graph::setPage(ReportPage *p) {
	page = p;
}
//============================================================================
void Graph::setTitle(string t) {
	title = t;
}
//============================================================================
void Graph::setVertLabel(string l) {
	vertLabel = l;
}
//============================================================================
void Graph::setHorzLabel(string l) {
	horzLabel = l;
}
//============================================================================
void Graph::setVertStart(float start) {
	vertStart = start;
}
//============================================================================
void Graph::setHorzStart(float start) {
	horzStart = start;
}
//============================================================================
void Graph::setVertEnd(float end) {
	vertEnd = end;
}
//============================================================================
void Graph::setHorzEnd(float end) {
	horzEnd = end;
}
//============================================================================
void Graph::setVertJump(float jump) {
	vertJump = jump;
}
//============================================================================
void Graph::setHorzJump(float jump) {
	horzJump = jump;
}
//============================================================================
void Graph::setVertScale(float scale) {
	vertScale = scale;
}
//============================================================================
void Graph::setHorzScale(float scale) {
	horzScale = scale;
}
//============================================================================
void Graph::draw(HPDF_REAL x, HPDF_REAL y, GraphStyleEnum style) {
	// Define colors by style.
	HPDF_REAL bgColor[3];
	HPDF_REAL fgColor[3];
	HPDF_REAL lineColor[3];

	switch(style) {
	case defaultGraph:
		bgColor[0] = 1.0; bgColor[1] = 1.0; bgColor[2] = 1.0;
		fgColor[0] = 1.0; fgColor[1] = 1.0; fgColor[2] = 1.0;
		lineColor[0] = 0.8; lineColor[1] = 0.8; lineColor[2] = 0.8;
		break;
	default:
		cout << "Error: Undefined graph style." << endl;
		bgColor[0] = 1.0; bgColor[1] = 1.0; bgColor[2] = 1.0;
		fgColor[0] = 1.0; fgColor[1] = 1.0; fgColor[2] = 1.0;
		lineColor[0] = 0.0; lineColor[1] = 0.0; lineColor[2] = 0.0;
		break;
	}

	int graphBoundsWidth = getWidth();
	int graphBoundsHeight = getHeight();
	int graphWidth = getXAxisLength();
	int graphHeight = getYAxisLength();
	int xLeftGraphEdge = x + gGraphPadding;
	int xRightGraphEdge = xLeftGraphEdge + graphWidth;
	int yTopOfGraph = y - gGraphPadding;
	int yBottomOfGraph = yTopOfGraph - graphHeight;

	xOrigin = xLeftGraphEdge;
	yOrigin = yBottomOfGraph;

	// Outer box of the graph.
	HPDF_Page_SetRGBStroke(page->pageHandle, 0.0, 0.0, 0.0);
	DrawBox(page->pageHandle, 1.0, x, y - graphBoundsHeight, graphBoundsWidth, graphBoundsHeight, BOX_OUTLINE_ALL);

	HPDF_Page_SetRGBStroke(page->pageHandle, bgColor[0], bgColor[1], bgColor[2]);
	DrawBoxFill(page->pageHandle, x, y - graphBoundsHeight, graphBoundsWidth, graphBoundsHeight);

	// Axes.
	HPDF_Page_SetRGBStroke(page->pageHandle, 0.0, 0.0, 0.0);
	DrawLine(page->pageHandle, 1.0, xLeftGraphEdge, yBottomOfGraph, xRightGraphEdge, yBottomOfGraph);
	DrawLine(page->pageHandle, 1.0, xLeftGraphEdge, yBottomOfGraph, xLeftGraphEdge, yTopOfGraph);

	// Axis complements.
	DrawLine(page->pageHandle, 1.0, xLeftGraphEdge, yTopOfGraph, xRightGraphEdge, yTopOfGraph);
	DrawLine(page->pageHandle, 1.0, xRightGraphEdge, y - graphHeight - gGraphPadding, xRightGraphEdge, yTopOfGraph);

	// Graph area color.
	HPDF_Page_SetRGBStroke(page->pageHandle, fgColor[0], fgColor[1], fgColor[2]);
	DrawBoxFill(page->pageHandle, xLeftGraphEdge, yBottomOfGraph, graphWidth, graphHeight);

	// Black text.
	HPDF_Page_SetRGBStroke(page->pageHandle, 0.0, 0.0, 0.0);

	// Text labels.
	HPDF_Page_BeginText(page->pageHandle);

	// Title.
	HPDF_Page_SetFontAndSize(page->pageHandle, FONT_DEFAULT, 14);
	//HPDF_Page_SetRGBFill(page->pageHandle, fontR, fontG, fontB);
	HPDF_REAL tw = HPDF_Page_TextWidth(page->pageHandle, title.c_str());
	HPDF_Page_TextOut(page->pageHandle, (graphBoundsWidth - tw)/2 + x, y - 30, title.c_str());

	// X-Axis label.
	HPDF_Page_SetFontAndSize(page->pageHandle, FONT_DEFAULT, 12);
	//HPDF_Page_SetRGBFill(page->pageHandle, fontR, fontG, fontB);
	tw = HPDF_Page_TextWidth(page->pageHandle, horzLabel.c_str());
	HPDF_Page_TextOut(page->pageHandle, (graphWidth - tw)/2 + xLeftGraphEdge, yBottomOfGraph - 40, horzLabel.c_str());

	// Y-Axis label.
	tw = HPDF_Page_TextWidth(page->pageHandle, vertLabel.c_str());
	HPDF_Page_SetTextMatrix (page->pageHandle, cos(3.1415/2.0), sin(3.1415/2.0), -sin(3.1415/2.0), cos(3.1415/2.0), x + 15, (graphHeight - tw)/2 + yBottomOfGraph);
	HPDF_Page_SetFontAndSize(page->pageHandle, FONT_DEFAULT, 12);
	//HPDF_Page_SetRGBFill(page->pageHandle, fontR, fontG, fontB);
	HPDF_Page_ShowText(page->pageHandle, vertLabel.c_str());

	HPDF_Page_EndText(page->pageHandle);

	char txt[8];

	// X-Axis tick marks and number labels.
	for (int i = horzStart; i <= horzEnd; i += horzJump) {
		float scaled = i*horzScale;

		HPDF_Page_SetRGBStroke(page->pageHandle, 0.0, 0.0, 0.0);
		DrawLine(page->pageHandle, 1.0, xLeftGraphEdge + scaled, yBottomOfGraph, xLeftGraphEdge + scaled, yBottomOfGraph - gHorzTickLen);

		HPDF_Page_BeginText(page->pageHandle);
		HPDF_Page_SetFontAndSize(page->pageHandle, FONT_DEFAULT, 10);
		//HPDF_Page_SetRGBFill(page->pageHandle, fontR, fontG, fontB);
		sprintf(txt, "%d", i);
		HPDF_REAL tw = HPDF_Page_TextWidth(page->pageHandle, txt);
		HPDF_Page_TextOut(page->pageHandle, (xLeftGraphEdge - tw/2) + scaled, yBottomOfGraph - gHorzTickLen - gHorzTickLabelDist, txt);
		HPDF_Page_EndText(page->pageHandle);
	}

	// Y-Axis lines and number labels.
	for (int i = vertStart; i <= vertEnd; i += vertJump) {
		float scaled = i*vertScale;
		DrawLine(page->pageHandle, 1.0, xLeftGraphEdge - gVertTickLen, yBottomOfGraph + scaled, xLeftGraphEdge, yBottomOfGraph + scaled);

		// Horizontal line.
		HPDF_Page_SetRGBStroke(page->pageHandle, lineColor[0], lineColor[1], lineColor[2]);
		DrawLine(page->pageHandle, 1.0, xLeftGraphEdge, yBottomOfGraph + scaled, xRightGraphEdge, yBottomOfGraph + scaled);

		HPDF_Page_BeginText(page->pageHandle);
		HPDF_Page_SetFontAndSize(page->pageHandle, FONT_DEFAULT, 10);
		//HPDF_Page_SetRGBFill(page->pageHandle, fontR, fontG, fontB);
		sprintf(txt, "%d", i);
		HPDF_REAL tw = HPDF_Page_TextWidth(page->pageHandle, txt);
		HPDF_Page_TextOut(page->pageHandle, xLeftGraphEdge - tw - gVertTickLen - gVertTickLabelDist, yBottomOfGraph + scaled - 3, txt);
		HPDF_Page_EndText(page->pageHandle);
	}
}
//============================================================================
void Graph::drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, GraphStyleEnum style) {
	int width = getWidth();
	Graph::draw(((page->width-width) / 2) + centerOffset, y, style);
}
