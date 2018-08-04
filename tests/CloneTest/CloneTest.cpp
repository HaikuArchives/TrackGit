/**
 * @file CloneTest.cpp
 * @brief Implementaion file of Clone Test.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "CloneTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>

#include <AppKit.h>

#include "../../GitCommand/Clone.h"

#define PATH "/boot/home/CloneTest"
#define GIT_PATH "/boot/home/CloneTest/.git"
#define URL "https://github.com/Hrily/PythonImage"

CPPUNIT_TEST_SUITE_REGISTRATION(CloneTest);

void
CloneTest::setUp(void)
{
}

void
CloneTest::tearDown(void) 
{
	// Remove created repo
	char command[40];
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
CloneTest::test(void)
{
	// check git init results
	Clone clone(PATH, PATH);
	pthread_t threadId = clone.DoClone(NULL, URL, PATH);
	pthread_join(threadId, NULL);	
	struct stat sb;
	// Check if .git directory is created in PATH
	CPPUNIT_ASSERT (stat(GIT_PATH, &sb) == 0 && S_ISDIR(sb.st_mode));
}
