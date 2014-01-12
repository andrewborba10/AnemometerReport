//=========================================================
// ReportPage.h Jun 28, 2011
//=========================================================
#ifndef REPORTPAGE_H_
#define REPORTPAGE_H_
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <string.h>
#include <vector>
#include "hpdf.h"
#include "WindReport.h"
#include "ReportObject.h"
#include "Table.h"
#include "Graph.h"
#include "LineGraph.h"
#include "BarGraph.h"
#include "Token.h"
using namespace std;
//=========================================================
class ReportPage {
public:
	ReportPage(HPDF_Doc);
	void loadPageContent(string);
	void substituteTokens();
	void renderPageContent();

	HPDF_Doc pdfHandle;
	HPDF_Page pageHandle;
	HPDF_REAL height;
	HPDF_REAL width;

	void writeHeader(ReportObject);
	void writeText(ReportObject);
	void writeTable(ReportObject, HPDF_REAL, HPDF_REAL, int* = NULL, int* = NULL);
	void writeGraph(ReportObject, HPDF_REAL, HPDF_REAL, int* = NULL);

	friend class Table;

private:
	vector<ReportObject> objects;
};
//=========================================================
#endif /* REPORTPAGE_H_ */
