#include "File.h"

#include <fstream>

#include "StringHelper.h"
#include "FileHelper.h"
#include "../exception/NoiseStreamerException.h"

File::File(string filename) : filename(filename)
{
    _locker.init();
}

File::~File()
{
    clear();
    _locker.destroy();
}

int File::size()
{
    return lines.size();
}

void File::load()
{
    ifstream file(filename.c_str());
    if(file.fail())
    {
        throw DomainException(GNR0001, filename);
    }

    string line;
    while (getline(file, line))
    {
        _locker.lock();
        lines.push_back(line);
        _locker.unlock();
    }

    file.close();
}

void File::loadLast(int count)
{
    vector<string> fileContent = readLastLines(filename.c_str(), count);
    for (int i = 0; i < fileContent.size(); i++)
    {
        lines.push_back(fileContent.at(i));
    }
}

void File::loadRestOf(int count)
{
    if (count <= 0)
    {
        return;
    }

    int fileSize = lineCount(filename.c_str());
    int rest = fileSize % count;
    loadLast(rest);
}

void File::clear()
{
    _locker.lock();
    lines.clear();
    _locker.unlock();
}

void File::reload()
{
    clear();
    load();
}

void File::reloadLast(int count)
{
    clear();
    loadLast(count);
}

string File::read(int index)
{
    if (index >= size() || index < 0)
    {
        throw DomainException(GNR0002, numberToString<int>(index).c_str());
    }

    return lines[index];
}

int File::read(string line)
{
    for (int i = 0; i < size(); i++)
    {
        if (lines[i] == line)
        {
            return i;
        }
    }

    return -1;
}

void File::append(string line)
{
    _locker.lock();

    lines.push_back(line);
    appendLineFileToFile(filename.c_str(), line);

    _locker.unlock();
}
