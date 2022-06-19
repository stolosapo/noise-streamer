#ifndef MainTestSuite_h__
#define MainTestSuite_h__

#include <noisetest/UnitTestSuite.h>

class MainTestSuite: public NoiseTest::UnitTestSuite
{
protected:
	virtual void registerTests();

public:
	MainTestSuite();
	virtual ~MainTestSuite();

};

#endif // MainTestSuite_h__
