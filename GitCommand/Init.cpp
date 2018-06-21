/**
 * @file Init.cpp
 * @brief Implementaion file of Init command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Init.h"
#include "../Utils.h"

#include <InterfaceKit.h>

#include <git2.h>


/**
 * Init command constructor.
 * @param dirPath The directory where Init is selected.
 */
Init::Init(BString dirPath)
	:
	GitCommand()
{
	fDirPath = dirPath;
}


/**
 * Initializes empty repo in given directory.
 * @param dirPath The given directory.
 * @returns 0 or error code.
 */
int
Init::InitRepo(BString dirPath)
{
	git_repository* repo = NULL;
	return git_repository_init(&repo, dirPath.String(), 0);
}


/**
 * Init command excution. Initializes empty git repository in given directory.
 */
void
Init::Execute()
{
	if (InitRepo(fDirPath) < 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	} else {
		BString buffer("Repository initialted sucessfully.");
		BAlert* alert = new BAlert("", buffer.String(), "OK", 
			0, 0, B_WIDTH_AS_USUAL);
		alert->Go();
	}

	BMessenger messenger(APP_SIGN);
	messenger.SendMessage(new BMessage(kQuitWindow));
}
