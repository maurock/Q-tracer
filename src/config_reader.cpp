/*
 * config_reader.cpp
 *
 *  Created on: 7 giu 2020
 *      Author: mauro
 */

// Code Specific Header File(s)
#include "../include/config_reader.h"
using namespace std;

ConfigReader::ConfigReader(std::string path_) : path(path_) {
	std::ifstream infile(path_); //The input stream
	if (infile) {
		std::string line;
		std::getline(infile, line); // Skip first two lines
		std::getline(infile, line);
		while (std::getline(infile, line))
		{
			vector<string> results;
			std::istringstream iss(line);
			while (std::getline(iss, line, ':')) {
				line.erase(std::remove(line.begin(),line.end(),' '),line.end());
				results.push_back(line);
			}
			cfg[std::string(results[0])] = std::string(results[1]); //Store the result in the map
		}
	}else{
		std::cout << "The configuration file doesn't exist or the path is incorrect";
	}
	infile.close(); //Close the file stream
};
