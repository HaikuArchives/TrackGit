/**
 * @file StatusTest.cpp
 * @brief Implementaion file of Status Test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "StatusTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <SupportKit.h>

#include "../../GitCommand/Init.h"

#define PATH "/boot/home/StatusTest"

CPPUNIT_TEST_SUITE_REGISTRATION(StatusTest);

void
StatusTest::setUp(void)
{
	// Create a git repo at PATH
	Init::InitRepo(PATH);

	char command[1024];

	// Add initial files
	sprintf(command, "touch %s/mod", PATH);
	system(command);
	sprintf(command, "touch %s/del", PATH);
	system(command);

	// Add files to repo
	sprintf(command, "(cd %s && git add --all)", PATH);
	system(command);

	// Add initial commit
	sprintf(command, "(cd %s && git commit -m \"Initial commit\")", PATH);
	system(command);

	// Add a file
	sprintf(command, "touch %s/nf", PATH);
	system(command);
	sprintf(command, "(cd %s && git add nf)", PATH);
	system(command);

	// Delete a file
	sprintf(command, "rm -rf %s/del", PATH);
	system(command);

	// Modify a file
	sprintf(command, "echo \"Hello World\" > %s/mod", PATH);
	system(command);

	// Create new file
	sprintf(command, "touch %s/unt", PATH);
	system(command);

	// Initialize status object
	status = new Status(BString(PATH), BString(PATH));
}

void
StatusTest::tearDown(void) 
{
	char command[40];

	// Remove created repo
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
StatusTest::test(void)
{
	BString* statusText = status->GetStatusText();

	// Check for new file
	CPPUNIT_ASSERT (statusText->FindFirst("new file:  nf") != B_ERROR);

	// Check for deleted file
	CPPUNIT_ASSERT (statusText->FindFirst("deleted:   del") != B_ERROR);

	// Check for modified file
	CPPUNIT_ASSERT (statusText->FindFirst("modified:  mod") != B_ERROR);

	// Check for untracked file
	CPPUNIT_ASSERT (statusText->FindFirst("untracked: unt") != B_ERROR);
}
