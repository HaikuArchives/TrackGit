/**
 * @file AddTest.h
 * @brief Header file of Add test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _ADD_TEST_H_
#define _ADD_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../GitCommand/Status.h"

using namespace std;

class AddTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(AddTest);
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
