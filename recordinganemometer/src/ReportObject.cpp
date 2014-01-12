//============================================================================
// ReportObject.cpp Jul 15, 2011
//============================================================================
#include "ReportObject.h"
//============================================================================
ReportObject::ReportObject(ReportObjectEnum object)
	:objectType(object) {
}
//============================================================================
TextStyleEnum TextStyleStringToSymbol(string style) {
	if (style == "titleText") return titleText;
	if (style == "subtitleText") return subtitleText;
	if (style == "heading1") return heading1;
	if (style == "regularText") return regularText;
	if (style == "importantText") return importantText;
	if (style == "specialNote") return specialNote;

	cout << "Invalid text style: \"" << style << "\"." << endl;
	throw std::exception();
}

TableEnum TableStringToSymbol(string table) {
	if (table == "envparams") return EnvParameterTable;
	if (table == "power") return PowerTable;
	if (table == "idletime") return IdleTimeTable;
	if (table == "energygen") return EnergyGenTable;
	if (table == "bestdays") return BestDaysTable;
	if (table == "value") return ValueTable;
	if (table == "carbon") return CarbonTable;
	if (table == "subsidy") return SubsidyTable;
	if (table == "scale") return ScaleTable;

	cout << "Invalid table: \"" << table << "\"." << endl;
	throw std::exception();
}

TableStyleEnum TableStyleStringToSymbol(string table) {
	if (table == "default") return defaultTable;
	if (table == "plain") return plainTable;

	cout << "Invalid table style: \"" << table << "\"." << endl;
	throw std::exception();
}

GraphEnum GraphStringToSymbol(string graph) {
	//if (graph == "monthlygraph") return monthlyGraph;

	cout << "Invalid graph: \"" << graph << "\"." << endl;
	throw std::exception();
}

GraphStyleEnum GraphStyleStringToSymbol(string graph) {
	if (graph == "default") return defaultGraph;

	cout << "Invalid graph style: \"" << graph << "\"." << endl;
	throw std::exception();
}

bool ReportObject::parseStartTag(string startTag) {
	stringstream ss;
	ss << startTag;
	if (startTag.find("header") == 0) {
		ss.ignore(7);
		this->objectType = objectHeader;
		return true;
	}
	if (startTag.find("text") == 0) {
		ss.ignore(5);

		this->styleText = regularText; // Defaults
		this->centered = false;

		char key[32], value[128];
		while(!ss.eof()) {
			ss.getline(key, 32, '=');
			ss.getline(value, 128, ' ');
			if (strcmp(key, "style") == 0) this->styleText = TextStyleStringToSymbol(value);
			if (strcmp(key, "centered") == 0) this->centered = (bool)atoi(value);
		}
		this->objectType = objectText;
		return true;
	}
	if (startTag.find("table") == 0) {
		ss.ignore(6);

		this->table = EnvParameterTable; // Defaults
		this->styleTable = defaultTable;
		this->centered = true;

		char key[32], value[128];
		while(!ss.eof()) {
			ss.getline(key, 32, '=');
			ss.getline(value, 128, ' ');
			if (strcmp(key, "name") == 0) this->table = TableStringToSymbol(value);
			if (strcmp(key, "style") == 0) this->styleTable = TableStyleStringToSymbol(value);
			if (strcmp(key, "centered") == 0) this->centered = (bool)atoi(value);
		}
		this->objectType = objectTable;
		return true;
	}
	if (startTag.find("graph") == 0) {
		ss.ignore(6);

		//this->graph = testLineGraph; // Defaults
		this->styleGraph = defaultGraph;
		this->centered = true;

		char key[32], value[128];
		while(!ss.eof()) {
			ss.getline(key, 32, '=');
			ss.getline(value, 128, ' ');
			if (strcmp(key, "name") == 0) this->graph = GraphStringToSymbol(value);
			if (strcmp(key, "style") == 0) this->styleGraph = GraphStyleStringToSymbol(value);
			if (strcmp(key, "centered") == 0) this->centered = (bool)atoi(value);
		}
		this->objectType = objectGraph;
		return true;
	}
	if (startTag.find("scale") == 0) {
		this->objectType = objectScale;
		return true;
	}
	if (startTag.find("tab") == 0) {
		this->objectType = objectTab;
		return true;
	}
	if (startTag.find("/tab") == 0) {
		this->objectType = objectUntab;
		return true;
	}
	return false;
}
