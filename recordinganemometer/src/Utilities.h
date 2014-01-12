//============================================================================
// Utilities.h Jul 1, 2011
//============================================================================

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "hpdf.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include "AssumedEnvironment.h"
using namespace std;
//============================================================================
extern float windPower[255];
class AssumedEnvironment;
extern AssumedEnvironment assumedEnvironment;
//============================================================================
// Box outline properties
#define BOX_OUTLINE_UPPER (1<<0)
#define BOX_OUTLINE_LOWER (1<<1)
#define BOX_OUTLINE_LEFT (1<<2)
#define BOX_OUTLINE_RIGHT (1<<3)

// Presets
#define BOX_OUTLINE_HORIZONTAL (BOX_OUTLINE_UPPER|BOX_OUTLINE_LOWER)
#define BOX_OUTLINE_VERTICAL (BOX_OUTLINE_LEFT|BOX_OUTLINE_RIGHT)
#define BOX_OUTLINE_ALL (BOX_OUTLINE_HORIZONTAL|BOX_OUTLINE_VERTICAL)
//============================================================================
// Prototypes.
float wsToRealPow(int, bool);
float wsToPow(int, bool, bool, bool, bool, bool, bool);
void SimplifyString(string&);
char* AppendFileSuffix(char*, char*);
string AddCommasToNumber(float, int);
void DrawLine(HPDF_Page, float, float, float, float ,float);
void DrawBox(HPDF_Page, float, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL, int);
void DrawBoxFill(HPDF_Page, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL);
void DrawBoxTextLeft(HPDF_Page, const char *, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL);
void DrawBoxTextCenter(HPDF_Page, const char *, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL);
void DrawUpArrow(HPDF_Page, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL, HPDF_REAL);

#endif /* UTILITIES_H_ */
