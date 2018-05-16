#include "inittest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <AppKit.h>

#define PATH "/boot/home/inittest"
#define GIT_PATH "/boot/home/inittest/.git"

CPPUNIT_TEST_SUITE_REGISTRATION (inittest);

void inittest :: setUp (void)
{
	// set up test environment (initializing objects)
	new BApplication("application/x-vnd.Haiku-TrackGit");
	init = new Init(PATH);
}

void inittest :: tearDown (void) 
{
	// finally delete objects
	delete init;
	// Remove created repo
	char command[40];
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void inittest :: initTest (void)
{
	// check git init results
	init->Execute();
	struct stat sb;
	// Check if .git directory is created in PATH
	CPPUNIT_ASSERT (stat(GIT_PATH, &sb) == 0 && S_ISDIR(sb.st_mode));
}
