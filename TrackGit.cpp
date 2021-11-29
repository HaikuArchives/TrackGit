/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Alert.h>
#include <Application.h>
#include <AppKit.h>
#include <Catalog.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <StringList.h>
#include <SupportKit.h>
#include <MenuItem.h>

#include <add-ons/tracker/TrackerAddOn.h>

#include <git2.h>

#include "SwitchBranch.h"
#include "TrackGitApp.h"
#include "Utils.h"


#define B_TRANSLATION_CONTEXT "TrackGit"

extern "C" {
	void populate_menu (BMessage* msg, BMenu* menu, BHandler* handler);
	void message_received (BMessage* msg);
}


/**
 * process_ref definition for addon.
 * @param dir_ref The current directory ref.
 * @param msg BMessage containing refs to selected files.
 */
void 
process_refs(entry_ref dir_ref, BMessage* msg, void*)
{
	TrackGitApp::AboutWindow();
}


/**
 * Populates menu for this addon.
 * @param msg The passed BMessage. Contains refs to current dir and selected 
 * 			  files.
 * @param menu The pointer to Tracker menu.
 * @param handler The BHandler of Tracker. This should be target of added items.
 */
void
populate_menu(BMessage* msg, BMenu* menu, BHandler* handler)
{
	if (menu == NULL){
		BString buffer("Null menu");
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
				0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return;
	}

	// Remove Menu item if already exists.
	BMenuItem* item = menu->FindItem(ADDON_NAME);
	if (item != NULL)
		menu->RemoveItem(item);

	BMenu* submenu = new BMenu(ADDON_NAME);

	vector<char*> selected;
	extract_selected_paths(msg, selected);
	BString dirPath = extract_current_directory(msg);

	git_libgit2_init();
	git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);

	// Check if current directory is in git repo.
	if (git_repository_discover(&buf, dirPath.String(), 0, NULL) == 0) {
		// buf is git repo
		git_repository* repo = NULL;
		git_index* index;
		// Init repo
		git_repository_open_ext(&repo, dirPath.String(), 0, NULL);
		// Init index
		git_repository_index(&index, repo);
		if (git_index_has_conflicts(index)) {
			// Show conflicts menu item
			BMessage* showConflictsMsg = new BMessage(*msg);
			showConflictsMsg->AddInt32("addon_item_id", kShowConflicts);
			BMenuItem* showConflictsItem = new BMenuItem(
					B_TRANSLATE("Show conflicts" B_UTF8_ELLIPSIS),
					showConflictsMsg);
			submenu->AddItem(showConflictsItem);	
		}

		// Add Status menu item
		BMessage* statusMsg = new BMessage(*msg);
		statusMsg->AddInt32("addon_item_id", kStatus);
		BMenuItem* statusItem = new BMenuItem(
				B_TRANSLATE("Status" B_UTF8_ELLIPSIS), statusMsg);
		submenu->AddItem(statusItem);	

		// Add Commit menu item
		BMessage* commitMsg = new BMessage(*msg);
		commitMsg->AddInt32("addon_item_id", kCommit);
		BMenuItem* commitItem = new BMenuItem(
				B_TRANSLATE("Commit" B_UTF8_ELLIPSIS), commitMsg);
		submenu->AddItem(commitItem);

		// Add Pull menu item
		BMessage* pullMsg = new BMessage(*msg);
		pullMsg->AddInt32("addon_item_id", kPull);
		BMenuItem* pullItem = new BMenuItem(
				B_TRANSLATE("Pull" B_UTF8_ELLIPSIS), pullMsg);
		submenu->AddItem(pullItem);

		// Add Push menu item
		BMessage* pushMsg = new BMessage(*msg);
		pushMsg->AddInt32("addon_item_id", kPush);
		BMenuItem* pushItem = new BMenuItem(
				B_TRANSLATE("Push" B_UTF8_ELLIPSIS), pushMsg);
		submenu->AddItem(pushItem);

		// Add Branch menu
		BMenu* branchMenu = new BMenu("BranchMenu");

		BMessage* switchBranchMsg = new BMessage(*msg);
		switchBranchMsg->AddInt32("addon_item_id", kSwitchBranch);
		BMenuItem* branchMenuItem = new BMenuItem(branchMenu, switchBranchMsg);
		branchMenuItem->SetLabel(B_TRANSLATE("Branches" B_UTF8_ELLIPSIS));

		BStringList branches;
		BString branch;
		if (SwitchBranch::GetBranches(dirPath, &branches, &branch, repo) == 0)
			for (int i = 0; i < branches.CountStrings(); i++) {
				BMessage* branchMsg = new BMessage(*msg);
				branchMsg->AddInt32("addon_item_id", kSwitchBranch);
				branchMsg->AddString("branch", branches.StringAt(i));
				branchMenu->AddItem(new BMenuItem(branches.StringAt(i), branchMsg));
				branchMenu->ItemAt(i)->SetMarked(branch == branches.StringAt(i));
			}
		else
			branchMenu->AddItem(
				new BMenuItem(B_TRANSLATE("Switch branch" B_UTF8_ELLIPSIS),
					switchBranchMsg));
		branchMenu->AddSeparatorItem();

		BMessage* createBranchMsg = new BMessage(*msg);
		createBranchMsg->AddInt32("addon_item_id", kCreateBranch);
		branchMenu->AddItem(new BMenuItem(
				B_TRANSLATE("Create branch" B_UTF8_ELLIPSIS), createBranchMsg));
		submenu->AddItem(branchMenuItem);

		// Add Log menu item
		BMessage* logMsg = new BMessage(*msg);
		logMsg->AddInt32("addon_item_id", kLog);
		BMenuItem* logItem = new BMenuItem(
				B_TRANSLATE("Log" B_UTF8_ELLIPSIS), logMsg);
		submenu->AddItem(logItem);

		if (selected.size() > 0) {
			// Add "Add Files" menu item
			BMessage* addMsg = new BMessage(*msg);
			addMsg->AddInt32("addon_item_id", kAdd);
			BMenuItem* addItem = new BMenuItem(
					B_TRANSLATE("Add files"), addMsg);
			submenu->AddItem(addItem);
		} else {
			// Add "Add All Files" menu item
			BMessage* addMsg = new BMessage(*msg);
			addMsg->AddInt32("addon_item_id", kAddAll);
			BMenuItem* addItem = new BMenuItem(
					B_TRANSLATE("Add all files"), addMsg);
			submenu->AddItem(addItem);
		}
	} else if (strcmp(menu->Name(), "MenuBar") == 0) {
		// For git-less dirPath, don't add items to menubar
		delete submenu;
		submenu = NULL;
	} else {
		// dirPath does not belong to git repo, menu isn't menubar
		// Add Clone menu item
		BMessage* cloneMsg = new BMessage(*msg);
		cloneMsg->AddInt32("addon_item_id", kClone);
		BMenuItem* cloneItem = new BMenuItem(
				B_TRANSLATE("Clone" B_UTF8_ELLIPSIS), cloneMsg);
		submenu->AddItem(cloneItem);

		// Add "Init Here" only if no files are selected.
		if (selected.size() == 0) {
			// Add Init here
			BMessage* initMsg = new BMessage(*msg);
			initMsg->AddInt32("addon_item_id", kInitHere);
			BMenuItem* initItem = new BMenuItem(B_TRANSLATE("Init Here"), 
				initMsg);
			submenu->AddItem(initItem);
		}
	}

	if (submenu != NULL) {
		menu->AddItem(submenu);
		submenu->SetTargetForItems(handler);
	}

	git_buf_free(&buf);
	git_libgit2_shutdown();
}


/**
 * Handler for received messages.
 * @param msg The passed BMessage.
 */
void 
message_received(BMessage* msg)
{
	int32 itemId;

	if (msg->FindInt32("addon_item_id", &itemId) != B_OK)
		return;

	BMessenger messenger(APP_SIGN);

	if (!messenger.IsValid()) {
		be_roster->Launch(APP_SIGN);
		messenger = BMessenger(APP_SIGN);
	}
	messenger.SendMessage(msg);
}


/**
 * The main function of TrackGit.
 * This function launches the TrackGit App.
 */
int 
main()
{
	TrackGitApp app;
	app.Run();
	return 0;
}
