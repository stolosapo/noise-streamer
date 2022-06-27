#ifndef FileHelper_h__
#define FileHelper_h__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

static const char* FILE_DELIMITTER;

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
vector<string> readLastLines(const char* filename, int lineCount);

#endif // FileHelper_h__
