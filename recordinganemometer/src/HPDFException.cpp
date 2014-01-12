//============================================================================
// HPDFException.cpp Jul 1st, 2011
//============================================================================
#include "HPDFException.h"
//============================================================================
void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
	throw HPDFException((HPDF_UINT)error_no, (HPDF_UINT)detail_no);
}
//============================================================================
HPDFException::HPDFException(HPDF_UINT errNo, HPDF_UINT detNo)
	:errorNo(errNo), detailNo(detNo) {

}
//============================================================================
string HPDFException::what() {
	stringstream ss;
	ss << "ERROR: error_no=" << errorNo << " detail_no=" << detailNo;
	return ss.str();
}
//============================================================================
