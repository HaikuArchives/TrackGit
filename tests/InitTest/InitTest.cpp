#include "InitTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <AppKit.h>

#include "../../GitCommand/Init.h"

#define PATH "/boot/home/InitTest"
#define GIT_PATH "/boot/home/InitTest/.git"

CPPUNIT_TEST_SUITE_REGISTRATION(InitTest);

void
InitTest::setUp(void)
{
}

void
InitTest::tearDown(void) 
{
	// Remove created repo
	char command[40];
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
InitTest::test(void)
{
	// check git init results
	Init::InitRepo(PATH);
	struct stat sb;
	// Check if .git directory is created in PATH
	CPPUNIT_ASSERT (stat(GIT_PATH, &sb) == 0 && S_ISDIR(sb.st_mode));
}
