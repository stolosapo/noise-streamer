#ifndef File_h__
#define File_h__

#include <string>
#include <vector>
#include <noisekernel/Thread.h>

using namespace std;
using namespace NoiseKernel;

class File
{
private:
    Locker _locker;
    string filename;
    vector<string> lines;

public:
    File(string filename);
    virtual ~File();

    int size();

    void load();
    void loadLast(int count);
    void loadRestOf(int count);
    void clear();
    void reload();
    void reloadLast(int count);

    string read(int index);
    int read(string line);
    void append(string line);
};

#endif // File_h__
