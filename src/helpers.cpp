#include "helpers.h"

std::random_device rd;
std::mt19937 gen(rd());

int GenerateIntValue(int min_val, int max_val)
{
    std::uniform_int_distribution<> dist(min_val, max_val);
    return dist(gen);
}

string timeStampToHReadble(time_t timestamp)
{
    time_t a = timestamp;
    return ctime(&a);
}

bool is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c)
                                      { return !std::isdigit(c); }) == s.end();
}

bool file_exists(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

std::vector<string> split(string str)
{
    std::vector<string> internal;
    std::istringstream ss(str);

    string word;

    while (ss >> word)
    {
        internal.push_back(word);
    }

    return internal;
}