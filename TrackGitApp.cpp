#include "TrackGitApp.h"
#include "Utils.h"

#include <InterfaceKit.h>

#include "UI/TrackGitWindow.h"
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
	// If message is received for quitting the window
	if (msg->what == kQuitWindow) {
		BString repo;
		if (msg->FindString("repo", &repo) != B_OK)
			return;
		fRunningCommands.erase(repo);
		// If all windows are quit
		if (fRunningCommands.size() == 0)
			be_app->PostMessage(B_QUIT_REQUESTED);
		return;
	}

	vector<const char*> selected;
	extract_selected_paths(msg, selected);
	BString dirPath = extract_current_directory(msg);

	int32 itemId;
	if (msg->FindInt32("addon_item_id", &itemId) != B_OK)
		return;

	BString repo = get_root_of_repo(dirPath);
	// Check if window for selected repo already exits
	// If yes bring it to front
	if (fRunningCommands[repo]) {
		fRunningCommands[repo]->Activate(true);
		BWindow* window = fRunningCommands[repo];
		if (window->Lock()) {
			window->Activate(true);
			window->Unlock();
		}
		return;
	}

	GitCommand* gitCommand = NULL;

	switch (itemId) {
		case kClone:
			gitCommand = new Clone(repo, dirPath);
			break;
		case kInitHere:
			gitCommand = new Init(dirPath);
			break;
		case kStatus:
			gitCommand = new Status(repo, dirPath);
			break;
		default:
			break;
	}

	fRunningCommands[repo] = gitCommand->GetWindow();
	
	if (gitCommand)
		gitCommand->Execute();
}
