#pragma once
#include <string>
#include <istream>
#include <map>
#include <functional>
#include <sstream>
#include <iostream>

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
	virtual T parse(std::istream& file) {
		std::string currLine = "";
		std::cout << "parsing" << std::endl;
		while (std::getline(file, currLine)) {
			parseLine(currLine);
		}
		std::cout << "done parsing" << std::endl;
		T obj = build();
		cleanup();
		return obj;
	};
	void setParsers(std::map<std::string, std::function<void(const std::string&)>>& parsers) {
		miniParsers = parsers;
	};
protected:
	virtual T build() = 0;
	virtual void cleanup() = 0;
	std::map<std::string, std::function<void(const std::string&)>> miniParsers;
private:
	bool parseLine(const std::string& line) {
		std::string firstSymbol = "";
		std::istringstream lineStream = std::istringstream(line);
		if (lineStream >> firstSymbol) {

			if (miniParsers.find(firstSymbol) != miniParsers.end()) {
				miniParsers[firstSymbol](line);
			}
		}
		return true;
	};
};