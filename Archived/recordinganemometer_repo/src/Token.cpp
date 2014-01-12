//============================================================================
// Token.cpp Jul 18, 2011
//============================================================================
#include "Token.h"
//============================================================================
map<string, string> tokens;
//============================================================================
void SetTokenFloat(string token, float value, int precision) {
	tokens[token] = AddCommasToNumber(value, precision);
}
//============================================================================
void GetTokenInfo(string token, string& tokenFormatted, int& size) {
	stringstream ss;
	ss << "{" << token << "}";
	tokenFormatted = ss.str();
	size = ss.str().size();
}
