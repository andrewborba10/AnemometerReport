//============================================================================
// AssumedEnvironment.cpp Jun 28, 2011
//============================================================================
#include "AssumedEnvironment.h"
//============================================================================
AssumedEnvironment::AssumedEnvironment() {
	// Initialize all values.
	sitename = "<Not specified>";
	description = "<None>";
	latitude = 0.0;
	longitude = 0.0;
	altitude = 0.0;
	towerheight = 0.0;
	sweptarea = 0.0;
	efficiency = 0.0;
	electricityvalue = 0.0;
	cutinspeed = 0.0;
	cutoutspeed = 0.0;
	survivalspeed = 0.0;
	seasonaladj = 0.0;
	maxgenpwr = 0.0;
	subsidy = 0.0;
}
//============================================================================
bool AssumedEnvironment::ProcessFile(string fileName) {
	ifstream f(fileName.c_str());
	if (f.fail()) {
		cout << "ERROR: Can't open environment parameters file: \"" << fileName << "\"" << endl;
		return false;
	}

	char element_c[128], value_c[512];
	string element, value;
	while(!f.eof()) {
		if (f.peek() == '/') {
			f.ignore(640, '\n'); // Ignore this line.
			continue; // Eat comments
		}
		while (f.peek() == '\n') f.get();
		f.getline(element_c, 256, ':'); // Get parameter element name.
		if (!element_c[0]) {
			f.ignore(640, '\n'); // Ignore this line.
			continue; // Ignore blank lines.
		}
		element = string(element_c);
		while (f.peek() == ' ' || f.peek() == '\t') f.get(); // Eat whitespace
		f.getline(value_c, 256); // Get value of element.
		value = string(value_c);
		SimplifyString(element); // Convert to lowercase and strip spaces so we don't depend on the user for this.
		userinput[element] = string(element_c);
		envparams[element] = value; // Store in map.
	}
	f.close();
	return true;
}

// Makes sure the values are acceptable and copies them into the object data.
bool AssumedEnvironment::ValidateAndCopy() {
	stringstream* ss = new stringstream; // No idea why I can't reuse stringstream in a loop.. so I just recreate a new one each iteration.
	map<string, string>::iterator it;
	for (it = envparams.begin(); it != envparams.end(); it++) {
		ss->str(it->second);
		     if (it->first == "altitude") 			*ss >> altitude;
		else if (it->first == "cut-inspeed")		*ss >> cutinspeed;
		else if (it->first == "cut-outspeed")		*ss >> cutoutspeed;
		else if (it->first == "description")		description = it->second;
		else if (it->first == "efficiency")			*ss >> efficiency;
		else if (it->first == "electricityvalue")	*ss >> electricityvalue;
		else if (it->first == "latitude")			*ss >> latitude;
		else if (it->first == "longitude")			*ss >> longitude;
		else if (it->first == "maxgeneratorpower")	*ss >> maxgenpwr;
		else if (it->first == "seasonaladjustment")	*ss >> seasonaladj;
		else if (it->first == "sitename")			sitename = it->second;
		else if (it->first == "subsidy")			*ss >> subsidy;
		else if (it->first == "survivalspeed")		*ss >> survivalspeed;
		else if (it->first == "sweptarea")			*ss >> sweptarea;
		else if (it->first == "towerheight")		*ss >> towerheight;
		else {
			cout << "Invalid environment parameter: \"" << userinput[it->first] << "\"" << endl;
			envparams.erase(it--);
		}

		if (ss->fail()) {
			cout << "Environment parameter \"" << userinput[it->first] << "\" has invalid value: \"" << it->second << "\"" << endl;
			return false;
		}

		// Add as a token.
		SetTokenValue<string>(it->first, it->second);

		delete ss;
		ss = new stringstream;
	}
	delete ss;

	// Loosely validate values:
	if (towerheight < 0) cout << "WARNING: Tower height is a negative value!" << endl;
	if (sweptarea < 0) cout << "WARNING: Swept area is a negative value!" << endl;
	if (efficiency < 0) cout << "WARNING: Efficiency is a negative value!" << endl;
	if (electricityvalue < 0) cout << "WARNING: Electricity value is a negative value!" << endl;
	if (cutinspeed < 0) cout << "WARNING: Cut-in speed is a negative value!" << endl;
	if (cutoutspeed < 0) cout << "WARNING: Cut-out speed is a negative value!" << endl;
	if (survivalspeed < 0) cout << "WARNING: Survival speed is a negative value!" << endl;
	if (maxgenpwr < 0) cout << "WARNING: Max generator power is a negative value!" << endl;
	if (subsidy < 0) cout << "WARNING: Subsidy is a negative value!" << endl;
	return true;
}
//============================================================================
