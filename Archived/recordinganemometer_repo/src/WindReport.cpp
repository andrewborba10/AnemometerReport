//============================================================================
// WindReport.cpp Jun 28, 2011
//============================================================================
#include "WindReport.h"
//============================================================================
HPDF_Font gFonts[4];
//============================================================================
WindReport::WindReport() {
}
//============================================================================
// Available Fonts:
// Courier, Courier-Bold, Courier-Oblique, Courier-BoldOblique
// Helvetica, Helvetica-Bold, Helvetica-Oblique, Helvetica-BoldOblique
// Times-Roman, Times-Bold, Times-Italic, Times-BoldItalic, Symbol
//============================================================================
bool WindReport::makeReport(WindDataSet wds, char *saveFileName) {
	try {
		// Document preparation.
		pdf = HPDF_New(error_handler, NULL);
		if (!pdf){
			cout << "Cannot create PdfDoc object" << endl;
			return false;
		}
		HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
		HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_OUTLINE);

		// Get fonts.
		FONT_DEFAULT = HPDF_GetFont(pdf, "Helvetica", NULL);
		FONT_BOLD = HPDF_GetFont(pdf, "Helvetica-Bold", NULL);
		FONT_OBLIQUE = HPDF_GetFont(pdf, "Helvetica-Oblique", NULL);
		FONT_BOLDOBLIQUE = HPDF_GetFont(pdf, "Helvetica-BoldOblique", NULL);

		doTitlePage();
		doParameterPage();
		doSummaryPage();
		doGeographicalComparisonPage();
		doPowerPage();
		doCapacityPage();
		doIdleTimePage();
		doBestDaysPage();
		doValuePage();
		doTowerPage();
		doSubsidyPage();
		doDailyPerformancePage();
		doConclusionsPage();

		// Save document to disc and free everything from memory.
		HPDF_SaveToFile(pdf, saveFileName);
		HPDF_Free(pdf);
	} catch (HPDFException &e) { // Something went wrong!
		cout << e.what() << endl;
		HPDF_Free(pdf);
	}
	return true;
}
//============================================================================
void WindReport::doTitlePage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/TitlePage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doParameterPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/ParameterPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doSummaryPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/SummaryPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doGeographicalComparisonPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/GeoComparisonPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doPowerPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/PowerPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doCapacityPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/CapacityPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doIdleTimePage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/IdleTimePage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doBestDaysPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/BestDaysPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doValuePage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/ValuePage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doTowerPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/TowerPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doSubsidyPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/SubsidyPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doDailyPerformancePage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/DailyPerfPage.txt");
	page.renderPageContent();
}
//============================================================================
void WindReport::doConclusionsPage() {
	ReportPage page(pdf);
	page.loadPageContent("Document Content/ConclusionsPage.txt");
	page.renderPageContent();

	//DrawUpArrow(page.pageHandle, 1.0, page.width/2, page.height/2, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5);
}
