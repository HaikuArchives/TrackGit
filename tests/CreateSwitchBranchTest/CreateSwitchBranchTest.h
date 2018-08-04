/**
 * @file CreateSwitchBranchTest.h
 * @brief Header file of Create and Switch Brach test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CREATE_SWITCH_BRANCH_TEST_H_
#define _CREATE_SWITCH_BRANCH_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../GitCommand/Status.h"

using namespace std;

class CreateSwitchBranchTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CreateSwitchBranchTest);
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
