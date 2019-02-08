#include "TextLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::stringstream;

string TextLoader::load(string path) {
	ifstream file;
	file.open(path);
	if (!file.is_open()) {
		std::cerr << "Problem opening file " << path << std::endl;
		return "";
	}
	stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}