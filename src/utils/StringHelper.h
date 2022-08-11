#ifndef StringHelper_h__
#define StringHelper_h__

#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
T stringToNumber(const string &str);
template <typename T>
string numberToString(T Number);
string pad(const string& str, int times);
vector<string> split(const string& str, const string& delimiter);
void join(const vector<string>& v, char c, string& s);

template <typename T>
T stringToNumber(const string &str)
{
    istringstream ss(str);
    T result;
    return ss >> result ? result : 0;
}

template <typename T>
string numberToString(T Number)
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

#endif /* StringHelper_h__ */
