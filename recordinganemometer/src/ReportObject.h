//============================================================================
// ReportObject.h Jul 15, 2011
//============================================================================
#ifndef REPORTOBJECTS_H_
#define REPORTOBJECTS_H_
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include "hpdf.h"
using namespace std;
//============================================================================
enum ReportObjectEnum {
	objectHeader = 0,
	objectText,
	objectNewline,
	objectTable,
	objectGraph,
	objectScale,
	objectTab,
	objectUntab,
};
//============================================================================
enum TextStyleEnum {
		titleText,
		subtitleText,
		heading1,
		regularText,
		importantText,
		specialNote,
};
//============================================================================
enum TableEnum {
		EnvParameterTable,
		PowerTable,
		IdleTimeTable,
		EnergyGenTable,
		BestDaysTable,
		ValueTable,
		CarbonTable,
		SubsidyTable,
		ScaleTable,
};
//============================================================================
enum TableStyleEnum {
		defaultTable,
		plainTable,
};
//============================================================================
enum GraphEnum {
};
//============================================================================
enum GraphStyleEnum {
		defaultGraph,
};
//============================================================================
TextStyleEnum TextStyleStringToSymbol(string);
TableEnum TableStringToSymbol(string);
TableStyleEnum TableStyleStringToSymbol(string);
GraphEnum GraphStringToSymbol(string);
GraphStyleEnum GraphStyleStringToSymbol(string);
//============================================================================
class ReportObject {
public:
	ReportObject() {}
	ReportObject(ReportObjectEnum);

	ReportObjectEnum objectType;
	bool centered;
	TextStyleEnum styleText;
	TableEnum table;
	TableStyleEnum styleTable;
	GraphEnum graph;
	GraphStyleEnum styleGraph;
	string text;


	bool parseStartTag(string);
};

#endif /* REPORTOBJECTS_H_ */
