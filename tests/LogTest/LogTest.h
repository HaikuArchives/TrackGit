#ifndef _LOG_TEST_H_
#define _LOG_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../GitCommand/Log.h"

using namespace std;

class LogTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(LogTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp(void);
		void tearDown(void);

	protected:
		void test(void);

	private:
		Log* log;
};

#endif
