#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "key.hpp"
#include "brute.hpp"

std::string me;
std::string encrypted;
std::string table_filename;
bool verbose = false;

Brute::Brute(const std::string& filename) {
	T.resize(N);
	std::string buffer;
    std::fstream input(filename.c_str(), std::ios::in);
    for (int i = 0; i < N; i++) {
        std::getline(input, buffer);
        T[i].set_string(buffer);
    }
    input.close();
}

void Brute::decrypt(const std::string& encrypted){
	printAllKLength(ALPHABET, "", encrypted.length(), encrypted);
}

void Brute::printAllKLength(const std::string& alphabet, const std::string&
	prefix, int k, const std::string& encrypted) {
	if (k == 0) {
		Key myObj(encrypted);
		Key abc(prefix);
		abc = abc.subset_sum(T, false); //change encryptedObj to abc, deleted Key
		if (abc == myObj) { //changed encryptedObj to abc
			std::cout << prefix << '\n';
		}
		return;
	}
	for (int x = 0; x < 31; ++x) {
		std::string myWord = prefix + alphabet[x];
		printAllKLength(alphabet, myWord, k - 1, encrypted);
	}
}

void usage(const std::string& error_msg="") {
	if (!error_msg.empty()) std::cout << "ERROR: " << error_msg << '\n';
	std::cout << me << ": Brute force cracking of Subset-sum password with "
		<< B << " bits precision\n"
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
	Brute brute(argv[2]);
	brute.decrypt(argv[1]);
	return 0;
}
