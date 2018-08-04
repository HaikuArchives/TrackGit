/**
 * @file LogTest.cpp
 * @brief Implementaion file of Log Test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "LogTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <SupportKit.h>

#include "../../GitCommand/Add.h"
#include "../../GitCommand/Init.h"
#include "../../GitCommand/Commit.h"

#define PATH "/boot/home/LogTest"
#define COMMIT_MSG "Initial commit"

CPPUNIT_TEST_SUITE_REGISTRATION(LogTest);

void
LogTest::setUp(void)
{
	// Create a git repo at PATH
	Init::InitRepo(PATH);

	char command[1024];

	// Add initial files
	sprintf(command, "touch %s/a", PATH);
	system(command);

	// Initialize status object
	log = new Log(BString(PATH));
}

void
LogTest::tearDown(void) 
{
	char command[40];

	// Remove created repo
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
LogTest::test(void)
{
	// Add file a
	vector<char*> files;
	files.push_back("a");
	Add::AddFiles(BString(PATH), files);

	// Commit the file
	Commit::DoCommit(BString(PATH), BString(COMMIT_MSG));

	// Check for commit in log
	BString logText = log->GetLogText();
	CPPUNIT_ASSERT (logText.FindFirst(COMMIT_MSG) != B_ERROR);
}
