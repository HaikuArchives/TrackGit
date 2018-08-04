/**
 * @file StatusTest.h
 * @brief Header file of Status test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _STATUS_TEST_H_
#define _STATUS_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../GitCommand/Status.h"

using namespace std;

class StatusTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(StatusTest);
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
