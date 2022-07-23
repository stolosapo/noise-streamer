#ifndef Excecutor_h__
#define Excecutor_h__

#include <string>

using namespace std;

class Excecutor
{
private:
    string pidFile;

    int createChildProcess();

public:
    Excecutor(string pidFile);
    virtual ~Excecutor();

    int forkAndExit();
    int forkAndWait();
    int forkAndExcecute(char* args[]);
};

#endif // Excecutor_h__
