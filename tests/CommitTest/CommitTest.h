#ifndef _COMMIT_TEST_H_
#define _COMMIT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../GitCommand/Status.h"

using namespace std;

class CommitTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CommitTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp(void);
		void tearDown(void);

	protected:
		void test(void);

	private:
		Status* status;
};

#endif
