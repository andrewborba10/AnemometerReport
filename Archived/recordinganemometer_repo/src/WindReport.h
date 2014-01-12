//============================================================================
// WindReport.h Jun 28, 2011
//============================================================================
#ifndef WINDREPORT_H_
#define WINDREPORT_H_
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <time.h>
#include "hpdf.h"
#include "HPDFException.h"
#include "ReportPage.h"
#include "WindDataSet.h"
#include "AssumedEnvironment.h"
#include "Table.h"
using namespace std;
//============================================================================
extern AssumedEnvironment assumedEnvironment;
extern WindDataSet wds;
//============================================================================
#define FONT_DEFAULT gFonts[0]
#define FONT_BOLD gFonts[1]
#define FONT_OBLIQUE gFonts[2]
#define FONT_BOLDOBLIQUE gFonts[3]
extern HPDF_Font gFonts[4];
//============================================================================
#define STD_HEADERHEIGHT 50
#define STD_HEADERFILLHEIGHT 30
#define STD_HEADERFILLPADDING 30
#define STD_STARTHEIGHT 75
#define STD_INDENT 25
//============================================================================
class WindReport {
public:
	WindReport();

	HPDF_Doc  pdf;
	bool makeReport(WindDataSet wds, char *saveFileName);

	friend class Table;

private:
    void createPage();
	void doTitlePage();
	void doParameterPage();
	void doSummaryPage();
	void doGeographicalComparisonPage();
	void doPowerPage();
	void doCapacityPage();
	void doIdleTimePage();
	void doBestDaysPage();
	void doValuePage();
	void doTowerPage( );
	void doSubsidyPage();
	void doDailyPerformancePage();
	void doConclusionsPage();
};
//============================================================================
#endif /* WINDREPORT_H_ */
