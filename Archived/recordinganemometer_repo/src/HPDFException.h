#ifndef HPDFEXCEPTION_H_
#define HPDFEXCEPTION_H_

#include <string>
#include <sstream>
#include "hpdf.h"
using namespace std;

void error_handler(HPDF_STATUS, HPDF_STATUS, void*);

class HPDFException {
private:
	HPDF_UINT errorNo;
	HPDF_UINT detailNo;
public:
	HPDFException(HPDF_UINT, HPDF_UINT);
	string what();
};

#endif /* HPDFEXCEPTION_H_ */
