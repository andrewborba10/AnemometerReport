//============================================================================
// RecordDay.cpp Jun 25, 2011
//============================================================================
#include "Table.h"
//============================================================================
Table::Table(ReportPage *rp)
	:page(rp) {
	clear();
}
//============================================================================
void Table::clear() {
	numRows = 0;
	for (int i = 0; i < 50; i++)
	{
		rowHeight[i] = 0;
		numCols[i] = 0;
		for (int j = 0; j < 10; j++) {
			cell[i][j] = "";
			cellCentered[i][j] = false;
		}
	}
	for (int i = 0; i < 10; i++) {
		colWidth[i] = 0;
	}
}
//============================================================================
int Table::addRow(string firstColText, bool centered) {
	cellCentered[numRows][0] = centered;
	cell[numRows][0] = firstColText;
	numCols[numRows] = 1;
	return numRows++;
}
//============================================================================
void Table::addCol(int row, string text, bool centered) {
	if (numCols[row] >= 10) {
		cout << "Too many columns added to table!  Row: " << row << " Text: " << text << endl;
		throw exception();
	}
	cellCentered[row][numCols[row]] = centered;
	cell[row][numCols[row]++] = text;
}
//============================================================================
void Table::setRowHeight(int height) {
	for (int i = 0; i < 50; i++)
		rowHeight[i] = height;
}
//============================================================================
void Table::setRowHeight(int row, int height) {
	rowHeight[row] = height;
}
//============================================================================
void Table::setColWidth(int width) {
	for (int i = 0; i < 10; i++)
		colWidth[i] = width;
}
//============================================================================
void Table::setColWidth(int col, int width) {
	colWidth[col] = width;
}
//============================================================================
int Table::getMaxWidth() {
	// Find the max width of the table.
	int rowWidth;
	int maxWidth = 0;
	for (int i = 0; i < numRows; i++) {
		rowWidth = 0;
		for (int j = 0; j < numCols[i]; j++) {
			rowWidth += colWidth[j];
		}
		if (rowWidth > maxWidth) maxWidth = rowWidth;
	}
	return maxWidth;
}
//============================================================================
int Table::getHeight() {
	int height = 0;
	for (int i = 0; i < numRows; i++) {
		height += rowHeight[i];
	}
	return height;
}
//============================================================================
// Build the table with the above methods and then call this.
void Table::draw(HPDF_REAL x, HPDF_REAL y, TableStyleEnum style) {
	// Values that determine the color, size, fill, etc of the graph.
	float fillR, fillG, fillB;
	float strokeR, strokeG, strokeB;
	float fontR, fontG, fontB;
	HPDF_Font font;
	HPDF_REAL size;

	int drawX = x;
	int drawY = y;
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols[i]; j++) {
			if (i == 0) { // Header row.
				switch(style) {
				case defaultTable:
					fillR = 0.0; fillG = 0.7; fillB = 0.7;
					strokeR = 0.0; strokeG = 0.0; strokeB = 0.0;
					fontR = 0.0; fontG = 0.0; fontB = 0.0;
					font = FONT_BOLD;
					size = 12;
					break;
				case plainTable:
					fillR = 1.0; fillG = 1.0; fillB = 1.0;
					strokeR = 0.0; strokeG = 0.0; strokeB = 0.0;
					fontR = 0.0; fontB = 0.0; fontB = 0.0;
					font = FONT_DEFAULT;
					size = 12;
					break;
				default:
					cout << "Error: Undefined table style." << endl;
					return;
				}
			}
			else { // Normal row.
				switch(style) {
					case defaultTable:
						fillR = 1.0; fillG = 1.0; fillB = 1.0;
						strokeR = 0.0; strokeG = 0.0; strokeB = 0.0;
						fontR = 0.0; fontG = 0.0; fontB = 0.0;
						font = FONT_DEFAULT;
						size = 12;
						break;
					case plainTable:
						fillR = 1.0; fillG = 1.0; fillB = 1.0;
						strokeR = 0.0; strokeG = 0.0; strokeB = 0.0;
						fontR = 0.0; fontG = 0.0; fontB = 0.0;
						font = FONT_DEFAULT;
						size = 12;
						break;
					default:
						cout << "Error: Undefined table style." << endl;
						return;
					}
			}
			HPDF_Page_SetRGBStroke(page->pageHandle, fillR, fillG, fillB);
			DrawBoxFill(page->pageHandle, drawX, drawY - rowHeight[i], colWidth[j], rowHeight[i]);
			HPDF_Page_SetRGBStroke(page->pageHandle, strokeR, strokeG, strokeB);
			DrawBox(page->pageHandle, 1.0, drawX, drawY - rowHeight[i], colWidth[j], rowHeight[i], BOX_OUTLINE_ALL);
			HPDF_Page_BeginText(page->pageHandle);
			HPDF_Page_SetFontAndSize(page->pageHandle, font, size);
			HPDF_Page_SetRGBFill(page->pageHandle, fontR, fontG, fontB);
			if (cellCentered[i][j])	DrawBoxTextCenter(page->pageHandle, cell[i][j].c_str(), drawX, drawY - rowHeight[i], colWidth[j], 5);
			else					DrawBoxTextLeft(page->pageHandle, cell[i][j].c_str(), drawX, drawY - rowHeight[i], 5, 5);
			HPDF_Page_EndText(page->pageHandle);
			drawX += colWidth[j];
		}
		drawX = x;
		drawY -= rowHeight[i];
	}
}
//============================================================================
void Table::drawCenter(HPDF_REAL centerOffset, HPDF_REAL y, TableStyleEnum style) { // Convenience function for printing a centered table.
	int maxWidth = getMaxWidth();
	draw(((page->width-maxWidth) / 2) + centerOffset, y, style);
}
