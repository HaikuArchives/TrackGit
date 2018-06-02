#include "TrackGitApp.h"
#include "Utils.h"

#include <InterfaceKit.h>
#include <SupportKit.h>

#include "GitCommand/GitCommand.h"
#include "GitCommand/Clone.h"
#include "GitCommand/Init.h"
#include "GitCommand/Status.h"


TrackGitApp::TrackGitApp()
	: 
	BApplication(APP_SIGN)
{
}


void
TrackGitApp::MessageReceived(BMessage* msg)
{
	int32 itemId;
	GitCommand* gitCommand = NULL;

	vector<const char*>* selected = new vector<const char*>();
	char* dirPath = get_selected(msg, selected);

	if (dirPath == NULL)
		return;
	if (msg->FindInt32("addon_item_id", &itemId) != B_OK)
		return;

	switch (itemId) {
		case kClone:
			gitCommand = new Clone(dirPath);
			break;
		case kInitHere:
			gitCommand = new Init(dirPath);
			break;
		case kStatus:
			gitCommand = new Status(dirPath);
			break;
		default:
			break;
	}
	printf("Window %d\n", windowMap[itemId]);

	// Check if window for selected option already exits
	// If yes bring it to front
	if (windowMap[itemId]) {
		printf("Activating Window\n");
		windowMap[itemId]->Activate(true);
		return;
	}

	windowMap[itemId] = gitCommand->GetWindow();
	printf("Window1 %d\n", windowMap[itemId]);
	if (gitCommand)
		gitCommand->Execute();
}
