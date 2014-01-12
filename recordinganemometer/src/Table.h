//============================================================================
// Table Jul 14, 2011
//============================================================================
#ifndef TABLE_H_
#define TABLE_H_
//============================================================================
#include "hpdf.h"
#include "WindReport.h"
#include "ReportObject.h"
#include "Utilities.h"
//============================================================================
class ReportPage;
//============================================================================
class Table {
public:
	Table() {}
	Table(ReportPage*);

private:
	ReportPage *page;
	string cell[50][10];
	bool cellCentered[50][10];
	int rowHeight[50];
	int colWidth[10];
	int numRows;
	int numCols[50];

public:
	void clear();
	int addRow(string, bool centered = true);
	void addCol(int, string, bool centered = true);
	void setRowHeight(int);
	void setRowHeight(int, int);
	void setColWidth(int);
	void setColWidth(int, int);
	int getMaxWidth();
	int getHeight();
	void draw(HPDF_REAL, HPDF_REAL, TableStyleEnum);
	void drawCenter(HPDF_REAL, HPDF_REAL, TableStyleEnum);
};

#endif /* TABLE_H_ */
