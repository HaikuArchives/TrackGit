/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "TrackGitApp.h"

#include <AboutWindow.h>
#include <Catalog.h>
#include <InterfaceKit.h>

#include "GitCommand/GitCommand.h"
#include "GitCommand/Clone.h"
#include "GitCommand/Init.h"
#include "GitCommand/Status.h"
#include "GitCommand/Add.h"
#include "GitCommand/Commit.h"
#include "GitCommand/Pull.h"
#include "GitCommand/Push.h"
#include "GitCommand/ShowConflicts.h"
#include "GitCommand/CreateBranch.h"
#include "GitCommand/SwitchBranch.h"
#include "GitCommand/Log.h"
#include "UI/TrackGitWindow.h"
#include "Utils.h"


#define B_TRANSLATION_CONTEXT "TrackGitApp"

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
			break;
		case kPull:
			gitCommand = new Pull(repo);
			break;
		case kPush:
			gitCommand = new Push(repo);
			break;
		case kShowConflicts:
			gitCommand = new ShowConflicts(repo);
			break;
		case kCreateBranch:
			gitCommand = new CreateBranch(repo);
			break;
		case kSwitchBranch:
			gitCommand = new SwitchBranch(repo, msg);
			break;
		case kLog:
			gitCommand = new Log(repo);
			break;
		default:
			break;
	}

	TrackGitWindow* window = gitCommand->GetWindow();
	if (window != NULL)
		fRunningCommands[repo] = window;
	
	if (gitCommand)
		gitCommand->Execute();
}


void
TrackGitApp::AboutRequested()
{
	AboutWindow();
}


void
TrackGitApp::AboutWindow()
{
	const char* authors[] = {
		"Adrien Destugues",
		"Hrishikesh Hiraskar",
		"humdinger",
		"Janus",
		"Stephan Aßmus",
		"waddlesplash",
		NULL
	};
	BAboutWindow* aboutW = new BAboutWindow(B_TRANSLATE_SYSTEM_NAME(ADDON_NAME),
		APP_SIGN);
	aboutW->AddDescription(B_TRANSLATE("A Tracker Addon for Git Version Control System"));
	aboutW->AddCopyright(2018, "Hrishikesh Hiraskar");
	aboutW->AddAuthors(authors);
	aboutW->Show();
}
