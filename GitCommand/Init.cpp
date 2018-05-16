#include "Init.h"

#include <InterfaceKit.h>

#include <git2.h>


Init::Init(const char* dirPath)
	:
	GitCommand()
{
	this->dirPath = dirPath;
}


void
Init::Execute()
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

	BString buffer("Repository initialted sucessfully.");
	BAlert* alert = new BAlert("", buffer.String(), "OK", 
		0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
}
