#include "CreateSwitchBranchTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <SupportKit.h>

#include "../../GitCommand/Init.h"
#include "../../GitCommand/CreateBranch.h"
#include "../../GitCommand/SwitchBranch.h"

#define PATH "/boot/home/CreateSwitchBranchTest"
#define BRANCH "testBranch"

CPPUNIT_TEST_SUITE_REGISTRATION(CreateSwitchBranchTest);

void
CreateSwitchBranchTest::setUp(void)
{
	// Create a git repo at PATH
	Init::InitRepo(PATH);

	char command[1024];

	// add initial files
	sprintf(command, "touch %s/mod", PATH);
	system(command);
	sprintf(command, "touch %s/del", PATH);
	system(command);

	// add files to repo
	sprintf(command, "(cd %s && git add --all)", PATH);
	system(command);

	// add initial commit necessary to branch
	sprintf(command, "(cd %s && git commit -m \"initial commit\")", PATH);
	system(command);

	// Initialize status object
	status = new Status(BString(PATH), BString(PATH));
}

void
CreateSwitchBranchTest::tearDown(void) 
{
	char command[100];

	// Remove created repo
	sprintf(command, "rm -rf %s", PATH);
	system(command);
}

void
CreateSwitchBranchTest::test(void)
{
	// Create Branch
	CreateBranch::DoCreateBranch(BString(PATH), BString(BRANCH));

	// Switch Branch
	SwitchBranch::DoSwitchBranch(BString(PATH), BString(BRANCH));

	BString* statusText = status->GetStatusText();
	printf("%s", statusText->String());

	BString branchText("Branch: %s");
	branchText.ReplaceFirst("%s", BRANCH);

	// Check if branch is created and switched
	CPPUNIT_ASSERT (statusText->FindFirst(branchText.String()) != B_ERROR);
}
