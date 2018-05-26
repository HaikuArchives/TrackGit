/**
 * @file Init.cpp
 * @brief Implementaion file of Init command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Init.h"

#include <InterfaceKit.h>

#include <git2.h>


/**
 * Init command constructor.
 * @param dirPath The directory where Init is selected.
 */
Init::Init(char* dirPath)
	:
	GitCommand()
{
	this->dirPath = dirPath;
}


/**
 * Initializes empty repo in given directory.
 * @param dirPath The given directory.
 */
void
Init::InitRepo(char* dirPath)
{
	git_repository* repo = NULL;
	if (git_repository_init(&repo, dirPath, 0) < 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return;
	}
}


/**
 * Init command excution. Initializes empty git repository in given directory.
 */
void
Init::Execute()
{
	InitRepo(dirPath);	

	BString buffer("Repository initialted sucessfully.");
	BAlert* alert = new BAlert("", buffer.String(), "OK", 
		0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
}
