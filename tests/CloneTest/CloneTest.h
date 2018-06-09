#ifndef _CLONE_TEST_H_
#define _CLONE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class CloneTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CloneTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp(void);
		void tearDown(void);

	protected:
		void test(void);
};

#endif
