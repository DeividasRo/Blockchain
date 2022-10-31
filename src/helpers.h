#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <algorithm>

using std::string;

// Generate integer value in a specified range
int GenerateIntValue(int min_val, int max_val);

// Convert integer to hex string
string IntToHexString(unsigned int value);

// Convert unix time to human readable date + time format
string TimeStampToHReadable(time_t timestamp);

// Check if string is numeric
bool is_number(const std::string &s);

// Check if file exists
bool file_exists(const char *fileName);

// Split string into vector of words
std::vector<string> split(string str);