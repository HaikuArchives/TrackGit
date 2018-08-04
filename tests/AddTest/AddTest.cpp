/**
 * @file AddTest.cpp
 * @brief Implementaion file of Add Test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "AddTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <SupportKit.h>

#include "../../GitCommand/Add.h"
#include "../../GitCommand/Init.h"

#define PATH "/boot/home/AddTest"

CPPUNIT_TEST_SUITE_REGISTRATION(AddTest);

void
AddTest::setUp(void)
{
	// Create a git repo at PATH
	Init::InitRepo(PATH);

	char command[1024];

	// Add initial files
	sprintf(command, "touch %s/a", PATH);
	system(command);
	sprintf(command, "touch %s/b", PATH);
	system(command);

	// Initialize status object
	status = new Status(BString(PATH), BString(PATH));
}

void
AddTest::tearDown(void) 
{
	char command[40];

	// Remove created repo
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
AddTest::test(void)
{
	// Add file a
	vector<char*> files;
	files.push_back("a");
	Add::AddFiles(BString(PATH), files);

	BString* statusText = status->GetStatusText();

	// Check for new file
	CPPUNIT_ASSERT (statusText->FindFirst("new file:  a") != B_ERROR);

	// Add all file
	files.clear();
	Add::AddFiles(BString(PATH), files);

	statusText = status->GetStatusText();

	// Check for new file
	CPPUNIT_ASSERT (statusText->FindFirst("new file:  b") != B_ERROR);
}
