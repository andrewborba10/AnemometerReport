//=========================================================
// ReportPage.cpp  Jun 28, 2011
//=========================================================
#include "ReportPage.h"
//=========================================================
ReportPage::ReportPage(HPDF_Doc d)
	:pdfHandle(d) {
	pageHandle = HPDF_AddPage(d);
	height = HPDF_Page_GetHeight(pageHandle);
	width = HPDF_Page_GetWidth(pageHandle);
}
//=========================================================
void ReportPage::loadPageContent(string fileName) {
	ifstream f(fileName.c_str());
	if (f.fail()) {
		cout << "ERROR: Can't find content file: \"" << fileName << "\"" << endl;
		return;
	}

	char buf[2048], rawStartTag[128], betweenTags[2048];

	while(!f.eof()) {
		// Turn newlines into newline report objects.
		while (f.peek() == '\n') {
			objects.push_back(ReportObject(objectNewline));
			f.get();
		}
		if (f.eof()) break;

		f.getline(buf, 2048, '<');
		f.getline(rawStartTag, 128, '>');
		ReportObject obj;
		if (!obj.parseStartTag(string(rawStartTag))) {
			cout << "Invalid tag: \"" << rawStartTag << "\"" << endl;
			continue;
		}

		switch(obj.objectType) {
		case objectHeader:
			f.getline(betweenTags, 2048, '<');
			obj.text = string(betweenTags);
			f.getline(buf, 2048);
			break;
		case objectText:
			f.getline(betweenTags, 2048, '<');
			obj.text = string(betweenTags);
			f.getline(buf, 2048);
			break;
		case objectTable:
			f.getline(buf, 2048);
			break;
		case objectGraph:
			f.getline(buf, 2048);
			break;
		case objectScale:
			f.getline(buf, 2048);
			break;
		case objectTab:
			f.getline(buf, 2048);
			break;
		case objectUntab:
			f.getline(buf, 2048);
			break;
		default:
			break;
		}

		objects.push_back(obj);
	}
}
//=========================================================
void ReportPage::substituteTokens() {
	map<string, string>::iterator it;
	size_t idx;
	stringstream ss;
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i].text.empty()) continue;

		// Substitute environment parameters.
		string tokenValue;
		string token;
		int size;
		for (it = tokens.begin(); it != tokens.end(); it++) {
			GetTokenInfo(it->first, token, size);
			idx = objects[i].text.find(token);
			if (idx != string::npos) objects[i].text.replace(idx, size, it->second);

		}
		for (it = assumedEnvironment.envparams.begin(); it != assumedEnvironment.envparams.end(); it++) {
			ss.str("");
			ss << "{" << it->first << "}";
			idx = objects[i].text.find(ss.str());
			if (idx != string::npos) objects[i].text.replace(idx, ss.str().size(), it->second);
		}
	}
}
//=========================================================
void ReportPage::renderPageContent() {
	substituteTokens();

	HPDF_Page_BeginText(pageHandle);
	HPDF_Page_MoveTextPos(pageHandle, 25, height - STD_STARTHEIGHT);

	HPDF_Point pos;
	ReportObject obj;
	int height;
	int width;
	float x;

	for (unsigned int i = 0; i < objects.size(); i++) {
		switch (objects[i].objectType) {
		case objectHeader:
			pos = HPDF_Page_GetCurrentTextPos(pageHandle);
			HPDF_Page_EndText(pageHandle);
			writeHeader(objects[i]);
			HPDF_Page_BeginText(pageHandle);
			HPDF_Page_MoveTextPos(pageHandle, pos.x, pos.y);

			break;
		case objectText:
			writeText(objects[i]);
			break;
		case objectNewline:
			HPDF_Page_MoveTextPos(pageHandle, 0, -15);
			break;
		case objectTab:
			HPDF_Page_MoveTextPos(pageHandle, STD_INDENT, 0);
			break;
		case objectUntab:
			HPDF_Page_MoveTextPos(pageHandle, -STD_INDENT, 0);
			break;
		case objectTable:
			pos = HPDF_Page_GetCurrentTextPos(pageHandle);
			HPDF_Page_EndText(pageHandle);
			writeTable(objects[i], pos.x, pos.y, &height);
			HPDF_Page_BeginText(pageHandle);
			HPDF_Page_MoveTextPos(pageHandle, pos.x, pos.y - height);
			break;
		case objectGraph:
			pos = HPDF_Page_GetCurrentTextPos(pageHandle);
			HPDF_Page_EndText(pageHandle);
			writeGraph(objects[i], pos.x, pos.y, &height);
			HPDF_Page_BeginText(pageHandle);
			HPDF_Page_MoveTextPos(pageHandle, pos.x, pos.y - height);
			break;
		case objectScale:
			pos = HPDF_Page_GetCurrentTextPos(pageHandle);
			HPDF_Page_EndText(pageHandle);
			obj.centered = true;
			obj.objectType = objectTable;
			obj.table = ScaleTable;
			obj.styleTable = plainTable;
			writeTable(obj, pos.x, pos.y, &height, &width);
			x = float(width) / 10.0;
			DrawUpArrow(pageHandle, 1.0, (this->width - width)/2 + x*wds.rating, pos.y - height, 0.0, 0.0, 0.0, 0.5, 0.0, 1.0);
			HPDF_Page_BeginText(pageHandle);
			HPDF_Page_MoveTextPos(pageHandle, pos.x, pos.y - height - 40);
			break;
		default:
			break;
		}
	}
	HPDF_Page_EndText(pageHandle);
}
//=========================================================
void ReportPage::writeHeader(ReportObject obj) {
	// Fill the lines.
	HPDF_Page_SetRGBStroke(pageHandle, 0.8, 0.8, 0.8);
	DrawBoxFill(pageHandle, STD_HEADERFILLPADDING, height - STD_HEADERHEIGHT, width - STD_HEADERFILLPADDING*2, STD_HEADERFILLHEIGHT);
	HPDF_Page_SetRGBStroke(pageHandle, 0.0, 0.0, 0.0);
	DrawBox(pageHandle, 1.0, STD_HEADERFILLPADDING, height - STD_HEADERHEIGHT, width - STD_HEADERFILLPADDING*2, STD_HEADERFILLHEIGHT, BOX_OUTLINE_LOWER|BOX_OUTLINE_LEFT);

	HPDF_Page_BeginText(pageHandle);
	HPDF_Page_SetFontAndSize(pageHandle, FONT_DEFAULT, 16);
	DrawBoxTextLeft(pageHandle, obj.text.c_str(), STD_HEADERFILLPADDING, height - STD_HEADERHEIGHT, 5, 5);
	HPDF_Page_EndText(pageHandle);
}
//=========================================================
void ReportPage::writeText(ReportObject obj) {
	if (obj.objectType != objectText) throw std::exception();

	char txt[1024];

	// Style stuff.
	HPDF_Page_SetTextLeading(pageHandle, 15);

	switch(obj.styleText) {
	case titleText:
		HPDF_Page_SetFontAndSize(pageHandle, FONT_DEFAULT, 24);
		HPDF_Page_SetRGBFill(pageHandle, 0.0, 0.0, 0.0);
		break;
	case subtitleText:
		HPDF_Page_SetFontAndSize(pageHandle, FONT_DEFAULT, 18);
		HPDF_Page_SetRGBFill(pageHandle, 0.0, 0.0, 0.0);
		break;
	case heading1:
		HPDF_Page_SetFontAndSize(pageHandle, FONT_BOLDOBLIQUE, 14);
		HPDF_Page_SetRGBFill(pageHandle, 0.45, 0.0, 0.0);
		break;
	case regularText:
		HPDF_Page_SetFontAndSize(pageHandle, FONT_DEFAULT, 12);
		HPDF_Page_SetRGBFill(pageHandle, 0.0, 0.0, 0.0);
		break;
	case importantText:
		HPDF_Page_SetFontAndSize(pageHandle, FONT_BOLD, 14);
		HPDF_Page_SetRGBFill(pageHandle, 0.45, 0.0, 0.0);
		break;
	case specialNote:
		HPDF_Page_SetFontAndSize(pageHandle, FONT_OBLIQUE, 12);
		HPDF_Page_SetRGBFill(pageHandle, 0.45, 0.0, 0.0);
		break;
	default:
		cout << "Error: undefined style for text." << endl;
		HPDF_Page_SetFontAndSize(pageHandle, FONT_DEFAULT, 12);
		HPDF_Page_SetRGBFill(pageHandle, 0.0, 0.0, 0.0);
		break;
	}

	strcpy(txt, obj.text.c_str());
	HPDF_Point pos = HPDF_Page_GetCurrentTextPos(pageHandle);
	if (obj.centered) {
		HPDF_Page_TextRect(pageHandle, pos.x, pos.y, width - 25, 0, txt, HPDF_TALIGN_CENTER, NULL);
		HPDF_Page_MoveTextPos(pageHandle, 0, 0); // Somehow fixes the x coordinate.  Possibly a bug in libharu.
		HPDF_Point pos2 = HPDF_Page_GetCurrentTextPos(pageHandle);
		HPDF_Page_MoveTextPos(pageHandle, pos.x - pos2.x, 0);
	}
	else {
		HPDF_Page_TextRect(pageHandle, pos.x, pos.y, width - 25, 0, txt, HPDF_TALIGN_LEFT, NULL);
	}
}
//=========================================================
void ReportPage::writeTable(ReportObject obj, HPDF_REAL x, HPDF_REAL y, int *tableHeight, int *tableWidth) {
	if (obj.objectType != objectTable) {
		cout << "writeTable can only handle table objects!" << endl;
		throw std::exception();
	}

	Table table(this);

	map<string, string>::iterator it;
	stringstream ss;
	stringstream ss2[15];
	char buf[32];
	int index;

	switch (obj.table) {
	case EnvParameterTable:
		// Define header info.
		table.addRow("Parameter");
		table.addCol(0, "Value");

		// Define column and row dimensions.
		table.setColWidth(0, 175);
		table.setColWidth(1, 250);
		table.setRowHeight(20);

		ss2[0] << assumedEnvironment.altitude << " m";
		ss2[1] << assumedEnvironment.cutinspeed << " m/s";
		ss2[2] << assumedEnvironment.cutoutspeed << " m/s";
		ss2[3] << assumedEnvironment.description;
		ss2[4] << assumedEnvironment.efficiency * 100 << "%";
		ss2[5] << "$" << assumedEnvironment.electricityvalue / 100.0;
		ss2[6] << fabs(assumedEnvironment.latitude) << " " << (char)((assumedEnvironment.latitude >= 0) ? 'N' : 'S');
		ss2[7] << fabs(assumedEnvironment.longitude) << " " << (char)((assumedEnvironment.longitude >= 0) ? 'E' : 'W');
		ss2[8] << assumedEnvironment.maxgenpwr / 1000.0 << " kw";
		ss2[9] << char((assumedEnvironment.seasonaladj >= 0) ? '+' : '-') << fabs(assumedEnvironment.seasonaladj) * 100 << "%";
		ss2[10] << assumedEnvironment.sitename;
		ss2[11] << "$" << fixed << setprecision(2) << assumedEnvironment.subsidy;
		ss2[12] << assumedEnvironment.survivalspeed << " m/s";
		ss2[13] << assumedEnvironment.sweptarea << " m^2" << endl;
		ss2[14] << assumedEnvironment.towerheight << " m" << endl;

		// Add all the environment parameters.
		table.addRow("Site Name"); table.addCol(1, ss2[10].str());
		table.addRow("Description"); table.addCol(2, ss2[3].str());
		table.addRow("Latitude"); table.addCol(3, ss2[6].str());
		table.addRow("Longitude"); table.addCol(4, ss2[7].str());
		table.addRow("Height above ground"); table.addCol(5, ss2[14].str());
		table.addRow("Absolute altitude"); table.addCol(6, ss2[0].str());
		table.addRow("Efficiency"); table.addCol(7, ss2[4].str());
		table.addRow("Cut-in (min) Speed"); table.addCol(8, ss2[1].str());
		table.addRow("Cut-out (max) Speed"); table.addCol(9, ss2[2].str());
		table.addRow("Survival Speed"); table.addCol(10, ss2[12].str());
		table.addRow("Swept Area"); table.addCol(11, ss2[13].str());
		table.addRow("Max Generator Power"); table.addCol(12, ss2[8].str());
		table.addRow("Seasonal Adjustment"); table.addCol(13, ss2[9].str());
		table.addRow("Electricity Value"); table.addCol(14, ss2[5].str());
		table.addRow("Subsidy"); table.addCol(15, ss2[11].str());

		break;
	case PowerTable:
		// Define header info.
		table.addRow("Wind Speed Range");
		table.addCol(0, "~ Wind Power Range");
		table.addCol(0, "Hours");

		// Define column and row height.
		table.setRowHeight(20);
		table.setColWidth(0, 125);
		table.setColWidth(1, 175);
		table.setColWidth(2, 75);

		for (unsigned int i = 0; i < wds.speedRanges.size() - 1; i++) {
			ss.str("");
			ss << fixed << setprecision(1) << wds.speedRanges[i]/10.0 << " - " << wds.speedRanges[i+1]/10.0 << " m/s";
			int row = table.addRow(ss.str());

			ss.str("");
			if (wds.wattRanges[i*2] != wds.wattRanges[i*2+1])
				ss << fixed << setprecision(3) << wds.wattRanges[i*2] << " - " << wds.wattRanges[i*2+1] << " kW";
			else
				ss << fixed << setprecision(3) << wds.wattRanges[i*2] << " kW";
			table.addCol(row, ss.str());

			ss.str("");
			//if (wds.exceededMax && i == wds.wattRanges.size()-2)
				//ss << wds.dailyHours[i] << ":" << setfill('0') << setw(2) << wds.dailyMinutes[i] << "*";
			//else
				ss << wds.dailyHours[i] << ":" << setfill('0') << setw(2) << wds.dailyMinutes[i];
			table.addCol(row, ss.str());
		}
		break;
	case IdleTimeTable:
		// Define header info.
		table.addRow("Duration");
		table.addCol(0, "# of Times");

		// Define column and row dimensions.
		table.setColWidth(0, 175);
		table.setColWidth(1, 125);
		table.setRowHeight(20);

		ss.str("");
		ss << wds.idleFor5Days;
		table.addRow("5 consecutive days"); table.addCol(1, ss.str());
		ss.str("");
		ss << wds.idleFor3Days;
		table.addRow("3 consecutive days"); table.addCol(2, ss.str());
		ss.str("");
		ss << wds.idleFor2Days;
		table.addRow("2 consecutive days"); table.addCol(3, ss.str());
		ss.str("");
		ss << wds.idleFor1Day;
		table.addRow("1 consecutive day"); table.addCol(4, ss.str());
		break;
	case EnergyGenTable:
		// Define header info.
		table.addRow("Energy generation");
		table.addCol(0, "# of Days");

		// Define column and row dimensions.
		table.setColWidth(0, 175);
		table.setColWidth(1, 125);
		table.setRowHeight(20);

		ss.str("");
		ss << wds.kwhFrom0to5Days;
		table.addRow("0  - 5 kw-hrs");  table.addCol(1, ss.str());

		ss.str("");
		ss << wds.kwhFrom6to10Days;
		table.addRow("5  - 10 kw-hrs"); table.addCol(2, ss.str());

		ss.str("");
		ss << wds.kwhFrom11to15Days;
		table.addRow("10 - 15 kw-hrs"); table.addCol(3, ss.str());

		ss.str("");
		ss << wds.kwhFrom16to20Days;
		table.addRow("15 - 20 kw-hrs"); table.addCol(4, ss.str());

		break;
	case BestDaysTable:
		// Define header info.
		table.addRow("Date");
		table.addCol(0, "Kw-hrs");

		// Define column and row dimensions.
		table.setColWidth(125);
		table.setRowHeight(20);

		for (int i = 0; i < wds.numBestDays; i++) {
			strftime(buf, 32, "%x", &wds.bestDayDateInfo[i]);
			ss.str("");
			ss << fixed << setprecision(2) << wds.bestDayEnergy[i];
			table.addRow(string(buf), true);  table.addCol(i+1, ss.str());
		}
		break;
	case ValueTable:
		// Define header info.
		table.addRow("Month");
		table.addCol(0, "Value");

		// Define column and row dimensions.
		table.setColWidth(125);
		table.setRowHeight(20);

		index = 1;
		for (int i = 0; i < 12; i++) {
			if (wds.monthlyElecValues[i] == 0) continue;
			ss.str("");
			ss << fixed << setprecision(2) << "$" << wds.monthlyElecValues[i];
			table.addRow(wds.monthNames[i]);  table.addCol(index++, ss.str());
		}
		break;
	case CarbonTable:
		// Define header info.
		table.addRow("Month");
		table.addCol(0, "Tons of Carbon");

		// Define column and row dimensions.
		table.setColWidth(125);
		table.setRowHeight(20);

		index = 1;
		for (int i = 0; i < 12; i++) {
			if (wds.monthlyCO2Values[i] == 0) continue;
			ss.str("");
			ss << fixed << setprecision(4) << wds.monthlyCO2Values[i];
			table.addRow(wds.monthNames[i]);  table.addCol(index++, ss.str());
		}
		break;
	case SubsidyTable:
		// Define header info.
		table.addRow("Subsidy/kw-hr in cents");
		table.addCol(0, "Value");

		// Define column and row dimensions.
		table.setColWidth(175);
		table.setRowHeight(20);

		table.addRow("2.0");  table.addCol(1, "<value>");
		table.addRow("2.5");  table.addCol(2, "<value>");
		table.addRow("3.0");  table.addCol(3, "<value>");
		table.addRow("3.5");  table.addCol(4, "<value>");
		table.addRow("4.0");  table.addCol(5, "<value>");
		table.addRow("4.5");  table.addCol(6, "<value>");
		table.addRow("5.0");  table.addCol(7, "<value>");
		break;
	case ScaleTable:
		// Define header info.
		table.addRow("Bad");
		table.addCol(0, "No");
		table.addCol(0, "Hmm");
		table.addCol(0, "Ok");
		table.addCol(0, "Fair");
		table.addCol(0, "Fine");
		table.addCol(0, "Good");
		table.addCol(0, "Great");
		table.addCol(0, "Wow");
		table.addCol(0, "Exclnt");

		// Define column and row dimensions.
		table.setColWidth(45);
		table.setRowHeight(25);
		break;
	default:
		break;
	}

	if (obj.centered) table.drawCenter(0, y, obj.styleTable);
	else table.draw(x, y, obj.styleTable);

	if (tableHeight != NULL) *tableHeight = table.getHeight();
	if (tableWidth != NULL) *tableWidth = table.getMaxWidth();
}

void ReportPage::writeGraph(ReportObject obj, HPDF_REAL x, HPDF_REAL y, int *graphHeight) {
	if (obj.objectType != objectGraph) {
		cout << "writeGraph can only handle graph objects!" << endl;
		throw std::exception();
	}

	LineGraph *linegraph = NULL;
	BarGraph *bargraph = NULL;

	switch (obj.graph) {
	/*
	case testLineGraph:
		linegraph = new LineGraph(this);
		linegraph->setTitle("Line Graph");
		linegraph->setVertLabel("Vertical!");
		linegraph->setHorzLabel("Horizontal!");
		linegraph->setVertStart(0.0);
		linegraph->setHorzStart(0.0);
		linegraph->setVertEnd(8.0);
		linegraph->setHorzEnd(6.0);
		linegraph->setVertJump(2.0);
		linegraph->setHorzJump(2.0);
		linegraph->setVertScale(15.0);
		linegraph->setHorzScale(40.0);

		// Push data points.
		linegraph->push(0, 1.0);
		linegraph->push(1, 5.0);
		linegraph->push(2, 6.0);
		linegraph->push(3, 7.0);
		linegraph->push(4, 7.5);
		break;
	case testBarGraph:
		bargraph = new BarGraph(this);
		bargraph->setTitle("Bar Graph");
		bargraph->setVertLabel("Vertical!");
		bargraph->setHorzLabel("Horizontal!");
		bargraph->setVertStart(0.0);
		bargraph->setHorzStart(0.0);
		bargraph->setVertEnd(10.0);
		bargraph->setHorzEnd(6.0);
		bargraph->setVertJump(1.0);
		bargraph->setHorzJump(1.0);
		bargraph->setVertScale(20.0);
		bargraph->setHorzScale(30.0);

		// Push data points.
		bargraph->push(0, 1.0);
		bargraph->push(1, 5.0);
		bargraph->push(2, 6.0);
		bargraph->push(3, 7.0);
		bargraph->push(4, 7.5);
		break;
	*/
	default:
		cout << "Invalid graph." << endl;
		break;
	}

	if (linegraph != NULL) {
		if (obj.centered) linegraph->drawCenter(0, y, obj.styleGraph);
		else linegraph->draw(x, y, obj.styleGraph);
		*graphHeight = linegraph->getHeight();
	}
	else if (bargraph != NULL) {
		if (obj.centered) bargraph->drawCenter(0, y, obj.styleGraph);
		else bargraph->draw(x, y, obj.styleGraph);
		*graphHeight = bargraph->getHeight();
	}
}
