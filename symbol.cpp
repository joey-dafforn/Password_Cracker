#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "key.hpp"
#include "symbol.hpp"

std::string me;
std::string encrypted;
std::string table_filename;
bool verbose = false;
std::map <Key, std::string> myMap;
std::string numOfAs;
int checker = 0;

Symbol::Symbol(const std::string& filename) {
	T.resize(N);
	std::string buffer;
    std::fstream input(filename.c_str(), std::ios::in);
    for (int i = 0; i < N; i++) {
        std::getline(input, buffer);
        T[i].set_string(buffer);
    }
    input.close();
}

void Symbol::decrypt(const std::string& encrypted){
	int myNum = encrypted.length();
	if (myNum % 2 != 0) { //odd
		for (int i = 0; i < (myNum + 1) / 2; i++) {
			numOfAs += "a";
		}
		if (checker == 1) {
			for (int i = 0; i < myNum / 2; i++) {
				numOfAs += "a";
			}
		}
	}
	else {
		for (int i = 0; i < myNum / 2; i++) {//even
			numOfAs += "a";
		}
	}
	if (myNum % 2 != 0) {
		printAllKLength(ALPHABET, "", myNum / 2, encrypted, T);
		differentPrintAllKLength(ALPHABET, "", (myNum + 1) / 2, encrypted, T);
	}
	else {
		printAllKLength(ALPHABET, "", myNum / 2, encrypted, T);
		differentPrintAllKLength(ALPHABET, "", myNum / 2, encrypted, T);
	}
}

void Symbol::printAllKLength(const std::string& alphabet, std::string
	prefix, int k, const std::string& encrypted, std::vector<Key> T) {
	if (k == 0) {
		prefix = numOfAs + prefix;
		Key myKey(prefix);
		Key myTempKey;
		myTempKey = myKey.subset_sum(T, false);
		myMap.insert(make_pair(myTempKey, prefix));
		return;
	}
	for (int x = 0; x < 32; ++x) {
		std::string myWord = prefix + alphabet[x];
		printAllKLength(alphabet, myWord, k - 1, encrypted, T);
	}
	checker = 1;
}

void Symbol::differentPrintAllKLength(const std::string& alphabet, std::string
	prefix, int k, const std::string& encrypted, std::vector<Key> T) {
	if (k == 0) {
		prefix = prefix + numOfAs;
		Key prefixKey(prefix);
		Key encryptedPrefixKey;
		encryptedPrefixKey = prefixKey.subset_sum(T, false); //key of encrypted value of prefix + aaaa
		std::string encryptedCopy = encrypted; // copy of encrypted
		Key encryptedCopyKey(encryptedCopy); //key of encrypted copy
		encryptedCopyKey -= encryptedPrefixKey;
		if (myMap.find(encryptedCopyKey) != myMap.end()){
			prefixKey.show();
			std::string myString = myMap.at(encryptedCopyKey);
			Key anotherKey(myString);
			anotherKey.show();
			anotherKey += prefixKey;
			anotherKey.showER();
		}
		return;
	}
	for (int x = 0; x < 32; ++x) {
		std::string myWord = prefix + alphabet[x];
		differentPrintAllKLength(alphabet, myWord, k - 1, encrypted, T);
	}
}
//loop through second half possibilites 32^c-first half length
//each time, make a key and incremenet second half
//prefix + myTempString
//main encrypted key - new encrypted key , check to see if in hash table
// get string value, get password

void usage(const std::string& error_msg="") {
	if (!error_msg.empty()) std::cout << "ERROR: " << error_msg << '\n';
	std::cout << me << ": Symbol table-based cracking of Subset-sum password"
		<< " with " << B << " bits precision\n"
	    << "USAGE: " << me << " <encrypted> <table file> [options]\n"
		<< "\nArguments:\n"
		<< " <encrypted>:   encrypted password to crack\n"
		<< " <table file>:  name of file containing the table to use\n"
		<< "\nOptions:\n"
		<< " -h|--help:     print this message\n"
		<< " -v|--verbose:  select verbose mode\n\n";
	exit(0);
}

void initialize(int argc, char* argv[]) {
	me = argv[0];
	if (argc < 3) usage("Missing arguments");
	encrypted = argv[1];
	table_filename = argv[2];
	for (int i=3; i<argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-h" || arg == "--help") usage();
		else if (arg == "-v" || arg == "--verbose") verbose = true;
		else usage("Unrecognized argument: " + arg);
	}
}

int main(int argc, char *argv[]){
	initialize(argc, argv);
	Symbol symbol(argv[2]);
	symbol.decrypt(argv[1]);
	//for (auto& p: myMap) //Prints out all elements in the map
		//std::cout << " " << p.first << " => " << p.second.at(0);
	return 0;
}
