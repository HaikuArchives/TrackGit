/**
 * @file CommitTest.cpp
 * @brief Implementaion file of Commit Test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "CommitTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <SupportKit.h>

#include "../../GitCommand/Add.h"
#include "../../GitCommand/Init.h"
#include "../../GitCommand/Commit.h"

#define PATH "/boot/home/CommitTest"

CPPUNIT_TEST_SUITE_REGISTRATION(CommitTest);

void
CommitTest::setUp(void)
{
	// Create a git repo at PATH
	Init::InitRepo(PATH);

	char command[1024];

	// Add initial files
	sprintf(command, "touch %s/a", PATH);
	system(command);

	// Initialize status object
	status = new Status(BString(PATH), BString(PATH));
}

void
CommitTest::tearDown(void) 
{
	char command[40];

	// Remove created repo
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
CommitTest::test(void)
{
	// Add file a
	vector<char*> files;
	files.push_back("a");
	Add::AddFiles(BString(PATH), files);

	// Check for new file
	BString* statusText = status->GetStatusText();
	CPPUNIT_ASSERT (statusText->FindFirst("new file:  a") != B_ERROR);

	Commit::DoCommit(BString(PATH), BString("Initial commit."));

	// The new file must be commited
	statusText = status->GetStatusText();
	CPPUNIT_ASSERT (statusText->FindFirst("new file:  a") == B_ERROR);
}
