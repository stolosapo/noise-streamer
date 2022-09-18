#include "StringHelper.h"

string pad(const string& str, int times)
{
    string mess = "";

    for (int i = 0; i < times; i++)
    {
        mess += str;
    }

    return mess;
}

vector<string> split(const string& str, const string& delimiter)
{
    vector<string> result;

    string s(str);

    size_t pos = 0;
    string token;

    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);

        result.push_back(token);

        s.erase(0, pos + delimiter.length());
    }

    result.push_back(s);

    return result;
}


void join(const vector<string>& v, char c, string& s)
{
    s.clear();

    for (vector<string>::const_iterator p = v.begin(); p != v.end(); ++p)
    {
        s += *p;
        if (p != v.end() - 1)
        {
            s += c;
        }
    }
}
