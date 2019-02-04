#pragma once
#include <string>
#include <istream>
#include <map>
#include <functional>
#include <sstream>
#include <iostream>

using std::string;
using std::getline;
using std::istream;
using std::map;
using std::function;
using std::istringstream;

template <class T>
class SimpleParser
{
public:
	explicit SimpleParser()
	{
	};
	virtual ~SimpleParser()
	{
	};
	virtual T parse(istream& file) {
		string currLine = "";
		std::cout << "parsing" << std::endl;
		while (getline(file, currLine)) {
			parseLine(currLine);
		}
		std::cout << "done parsing" << std::endl;
		T obj = build();
		cleanup();
		return obj;
	};
	void setParsers(map<string, function<void(const string&)>>& parsers) {
		miniParsers = parsers;
	};
protected:
	virtual T build() = 0;
	virtual void cleanup() = 0;
	map<string, function<void(const string&)>> miniParsers;
private:
	bool parseLine(const string& line) {
		string firstSymbol = "";
		istringstream lineStream = istringstream(line);
		if (lineStream >> firstSymbol) {

			if (miniParsers.find(firstSymbol) != miniParsers.end()) {
				miniParsers[firstSymbol](line);
			}
		}
		return true;
	};
};