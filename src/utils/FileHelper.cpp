#include "FileHelper.h"

#include "StringHelper.h"

#include <errno.h>
#include <fstream>
#include <cstring>

#include "../exception/NoiseStreamerException.h"

using namespace std;

static const char* FILE_DELIMITTER = "/";

FILE* openFile(string filename, const char* mode)
{
    if (!exists(filename.c_str()))
    {
        throw DomainException(GNR0001, filename);
    }

    return fopen(filename.c_str(), mode);
}

FILE* openReadBinary(string filename)
{
    return openFile(filename, "rb");
}

FILE* openWriteBinary(string filename)
{
    if (!exists(filename.c_str()))
    {
        createFile(filename);
    }

    return openFile(filename, "wb");
}

void createFile(string filename)
{
    ofstream outfile(filename.c_str());
    outfile.close();
}

bool exists(const char* filepath)
{
    ifstream f(filepath);
    return f.good();
}

string filenameExt(const char* filepath)
{
    vector<string> directories = split(string(filepath), string(FILE_DELIMITTER));
    return directories.back();
}

string filename(const char* filepath)
{
    string name = filenameExt(filepath);
    string ext = extension(name.c_str());
    if (ext == "")
    {
        return name;
    }

    ext = "." + ext;

    size_t pos = name.find(ext);

    return name.substr(0, pos);
}

string extension(const char* filename)
{
    string name = filenameExt(filename);
    vector<string> file = split(name, ".");
    if (file.size() <= 1)
    {
        return "";
    }

    return file.back();
}

int lineCount(const char* filename)
{
    ifstream file(filename);
    if(file.fail())
    {
        // throw DomainException(GeneralDomainErrorCode::GNR0001, filename);
        return 0;
    }

    int lineNumber = 0;
    string line;
    while (getline(file, line))
    {
        lineNumber++;
    }

    return lineNumber;
}

void appendLineFileToFile(const char* filename, string line)
{
    ofstream file;
    file.open(filename, ios::out | ios::app);

    if (file.fail())
    {
        throw ios_base::failure(strerror(errno));
    }

    //make sure write fails with exception if something is wrong
    file.exceptions(file.exceptions() | ios::failbit | ifstream::badbit);

    file << line << endl;

    file.close();
}

vector<string> readLastLines(const char* filename, int lineCount)
{
    vector<string> output;
    ifstream file(filename);

    if(file.fail())
    {
        // throw DomainException(GeneralDomainErrorCode::GNR0001, filename);
        return output;
    }

    vector<string> fileContent;
    string line;
    while (getline(file, line))
    {
        fileContent.push_back(line);
    }

    file.close();

    int it = 0;
    while (it < lineCount && !fileContent.empty())
    {
        output.insert(output.begin(), fileContent.back());
        fileContent.pop_back();
        it++;
    }

    return output;
}
