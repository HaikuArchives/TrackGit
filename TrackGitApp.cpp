/**
 * @file TrackGitApp.cpp
 * @brief Implementation file of TrackGitApp.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "TrackGitApp.h"
#include "Utils.h"

#include <InterfaceKit.h>

#include "UI/TrackGitWindow.h"
#include "GitCommand/GitCommand.h"
#include "GitCommand/Clone.h"
#include "GitCommand/Init.h"
#include "GitCommand/Status.h"
#include "GitCommand/Add.h"
#include "GitCommand/Commit.h"


/**
 * TrackGitApp Constructor.
 */
TrackGitApp::TrackGitApp()
	: 
	BApplication(APP_SIGN)
{
}


/**
 * The handler to receive messages.
 * This function acitivates already existing window or launches a new ones if 
 * needed. This also quits the app when there are no windows left.
 * @param msg The message to receive.
 */
void
TrackGitApp::MessageReceived(BMessage* msg)
{
	// If message is received for quitting the window
	if (msg->what == kQuitWindow) {
		BString repo;
		if (msg->FindString("repo", &repo) == B_OK)
			fRunningCommands.erase(repo);
		// If all windows are quit
		if (fRunningCommands.size() == 0)
			be_app->PostMessage(B_QUIT_REQUESTED);
		return;
	}

	vector<char*> selected;
	extract_selected_paths(msg, selected);
	BString dirPath = extract_current_directory(msg);

	int32 itemId;
	if (msg->FindInt32("addon_item_id", &itemId) != B_OK)
		return;

	BString repo = get_root_of_repo(dirPath);
	// Check if window for selected repo already exits
	// If yes bring it to front
	if (fRunningCommands.count(repo)) {
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
		case kAdd:
		case kAddAll:
			gitCommand = new Add(dirPath, selected);
			break;
		case kCommit:
			gitCommand = new Commit(repo);
		default:
			break;
	}

	TrackGitWindow* window = gitCommand->GetWindow();
	if (window != NULL)
		fRunningCommands[repo] = window;
	
	if (gitCommand)
		gitCommand->Execute();
}
