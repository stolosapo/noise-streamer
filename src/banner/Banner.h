#ifndef Banner_h__
#define Banner_h__

#include <vector>

using namespace std;

class Banner
{
private:
    vector<const char*> banners;

public:
    Banner();
    virtual ~Banner();

    const char* getRandom();
};

#endif // Banner_h__
