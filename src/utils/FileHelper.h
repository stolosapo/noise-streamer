#ifndef FileHelper_h__
#define FileHelper_h__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <errno.h>
#include <cstring>

using namespace std;

void createFile(string filename);

FILE* openFile(string filename, const char* mode);
FILE* openReadBinary(string filename);
FILE* openWriteBinary(string filename);

bool exists(const char* filepath);

string filenameExt(const char* filepath);
string filename(const char* filepath);
string extension(const char* filename);

int lineCount(const char* filename);

void appendLineFileToFile(const char* filename, string line);

template <typename T>
void overrideValueToFile(const char* filename, T value);
vector<string> readLastLines(const char* filename, int lineCount);


template <typename T>
void overrideValueToFile(const char* filename, T value)
{
    ofstream file;
    file.open(filename, ios::out | ios::trunc);

    if (file.fail())
    {
        throw ios_base::failure(strerror(errno));
    }

    //make sure write fails with exception if something is wrong
    file.exceptions(file.exceptions() | ios::failbit | ifstream::badbit);

    file << value << endl;

    file.close();
}

#endif // FileHelper_h__
