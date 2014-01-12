//============================================================================
// Utilities.cpp Jul 1, 2011
//============================================================================
#include "Utilities.h"
//============================================================================
// In kilowatts, this considers all turbine limitations
float wsToRealPow(int speed, bool kw) {
	return wsToPow(speed, kw, true, true, true, true, true);
}
//============================================================================
float wsToPow(int speed, bool kw, bool efficiency, bool sweptarea, bool cap, bool cutin, bool cutout) {
	float rawWindPower = windPower[speed];
	if (efficiency) rawWindPower *= assumedEnvironment.efficiency;
	if (sweptarea) rawWindPower *= assumedEnvironment.sweptarea;
	if (cap) rawWindPower = rawWindPower < assumedEnvironment.maxgenpwr ? rawWindPower : assumedEnvironment.maxgenpwr;
	if (cutin && speed < assumedEnvironment.cutinspeed * 10) rawWindPower = 0;
	if (cutout && speed > assumedEnvironment.cutoutspeed * 10) rawWindPower = 0;
	//cout << "cap " << cap << " efficiency " << efficiency << " sweptarea " << sweptarea << " WS " << speed << " = " << rawWindPower/1000.0 << " kW" << endl;
	return kw ? rawWindPower/1000.0 : rawWindPower;
}
//============================================================================
void SimplifyString(string& str)
{
	// Remove all spaces.
	size_t idx = 0;
	idx = str.find(' ', idx);
	while (idx != string::npos) {
		str.erase(idx, 1);
		idx = str.find(' ', idx);
	}

	// Make capitals into lowercase.
	for (unsigned int i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 'a' - 'A';
	}
}
//============================================================================
// Appends a file suffix if necessary.  If not it will return fileName.
string AppendFileSuffix(string fileName, string fileSuffix) {
	if (fileName.find("." + fileSuffix) == string::npos) {
		return fileName + "." + fileSuffix;
	}
	return fileName;
}
//============================================================================
// Add commas to a number
string AddCommasToNumber(float num, int precision) {
	ostringstream ss ;
	ss << fixed << setprecision(precision) << num ;
	string str = ss.str();
	int n = str.size() ;
	int start;
	unsigned int ppos = str.find('.');
	if (ppos == string::npos)  	start = n - 3;
	else						start = ppos - 3;
	for(int i = start; i > 0; i -= 3 ) str.insert(i, 1, ',') ;
	return str;
}
//============================================================================
// Draw a line given 2 points.
void DrawLine(HPDF_Page page, float thickness, float startx, float starty, float endx, float endy) {
	HPDF_Page_SetLineWidth(page, thickness);
	HPDF_Page_MoveTo(page, startx, starty);
	HPDF_Page_LineTo(page, endx, endy);
	HPDF_Page_Stroke(page);
}
//============================================================================
/**
 * The following functions go together.  For best results match the values of the parameters between
 * the different functions.  This can be used to print any kind of rectangular colored area
 * with text in it.  These can also be used to print cells of a table, see BOX_OUTLINE_* defines.
 */
//============================================================================
void DrawBox(HPDF_Page page, float lineThickness, HPDF_REAL x, HPDF_REAL y, HPDF_REAL width, HPDF_REAL height, int flags) {
	if (flags & BOX_OUTLINE_UPPER)
		DrawLine(page, lineThickness, x - (lineThickness / 2.0), y + height, x + width + (lineThickness / 2.0), y + height);
	if (flags & BOX_OUTLINE_LOWER)
		DrawLine(page, lineThickness, x - (lineThickness / 2.0), y, x + width + (lineThickness / 2.0), y);
	if (flags & BOX_OUTLINE_LEFT)
		DrawLine(page, lineThickness, x, y + height + (lineThickness / 2.0), x, y - (lineThickness / 2.0));
	if (flags & BOX_OUTLINE_RIGHT)
		DrawLine(page, lineThickness, x + width, y + height + (lineThickness / 2.0), x + width, y - (lineThickness / 2.0));
}

void DrawBoxFill(HPDF_Page page, HPDF_REAL x, HPDF_REAL y, HPDF_REAL width, HPDF_REAL height) {
	HPDF_Page_SetLineCap(page, HPDF_BUTT_END);
	DrawLine(page, height, x, y + (height/2.0), x + width, y + (height/2.0));
}

void DrawBoxTextLeft(HPDF_Page page, const char *txt, HPDF_REAL x, HPDF_REAL y, HPDF_REAL voffset, HPDF_REAL hoffset) {
	HPDF_Page_TextOut(page, x + hoffset, y + voffset, txt);
}

void DrawBoxTextCenter(HPDF_Page page, const char *txt, HPDF_REAL x, HPDF_REAL y, HPDF_REAL width, HPDF_REAL voffset ) {
	HPDF_REAL tw = HPDF_Page_TextWidth(page, txt);
	HPDF_Page_TextOut(page, x + (width - tw) / 2, y + voffset, txt);
}
//============================================================================
// x,y is the coordinate of the tip of the arrow.
void DrawUpArrow(HPDF_Page page, float lineThickness, HPDF_REAL x, HPDF_REAL y, HPDF_REAL rStroke, HPDF_REAL gStroke, HPDF_REAL bStroke, HPDF_REAL rFill, HPDF_REAL gFill, HPDF_REAL bFill) {
	HPDF_Page_SetLineWidth(page, lineThickness);
	HPDF_Page_SetRGBStroke (page, rStroke, gStroke, bStroke);
	HPDF_Page_SetRGBFill (page, rFill, gFill, bFill);
	HPDF_Page_MoveTo(page, x, y);
	HPDF_Page_LineTo(page, x+15, y-15);
	HPDF_Page_LineTo(page, x+7, y-15);
	HPDF_Page_LineTo(page, x+7, y-40);
	HPDF_Page_LineTo(page, x-7, y-40);
	HPDF_Page_LineTo(page, x-7, y-15);
	HPDF_Page_LineTo(page, x-15, y-15);
	HPDF_Page_LineTo(page, x, y);
	HPDF_Page_FillStroke(page);
}
