/*
 * config_reader.h
 *
 *  Created on: 7 giu 2020
 *      Author: mauro
 */

#ifndef INCLUDE_CONFIG_READER_H_
#define INCLUDE_CONFIG_READER_H_
#include <iostream>
#include <sstream>
#include <map>
#include <fstream>
#include <istream>
#include <algorithm>
#include <map>
#include <vector>

class ConfigReader
{
	public:
		std::string path;
		ConfigReader(std::string path_);
		std::map<std::string,std::string> cfg;   //A map of key-value pairs in the file
};

#endif /* INCLUDE_CONFIG_READER_H_ */
