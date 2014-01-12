//============================================================================
// Token.h Jul 18, 2011
//============================================================================
#ifndef TOKEN_H_
#define TOKEN_H_
//============================================================================
#include <map>
#include <sstream>
#include <string>
#include "Utilities.h"
using namespace std;
//============================================================================
extern map<string, string> tokens;
//============================================================================
// Templates.
template <typename T>
void SetTokenValue(string token,T value) {
	stringstream ss;
	ss << value;
	tokens[token] = ss.str();
}
//============================================================================
void SetTokenFloat(string token, float value, int precision = 0);
void GetTokenInfo(string token, string& tokenFormatted, int& size);

#endif /* TOKEN_H_ */
