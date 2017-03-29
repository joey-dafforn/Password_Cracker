#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "key.hpp"

class Symbol {
public:
	Symbol(const std::string&);
	void decrypt(const std::string&);
	void printAllKLength(const std::string&, std::string, int, const std::string&, std::vector<Key>);
	void differentPrintAllKLength(const std::string&, std::string, int, const std::string&, std::vector<Key>);
private:
	std::vector<Key> T;
};

#endif
