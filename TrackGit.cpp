/**
 * @file TrackGit.cpp
 * @brief Main file of TrackGit - A Tracker Addon for Git Version Control 
 * System.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include <Alert.h>
#include <Application.h>
#include <AppKit.h>
#include <Catalog.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <SupportKit.h>
#include <MenuItem.h>

#include <stdio.h>
#include <strings.h>
#include <vector>
#include <add-ons/tracker/TrackerAddOn.h>

#include "GitCommand/GitCommand.h"
#include "UI/CloneWindow.h"

#include "GitCommand/Clone.h"
#include "GitCommand/Init.h"

#include <git2.h>

#define B_TRANSLATION_CONTEXT "TrackGit"

extern "C" {
	void populate_menu (BMessage* msg, BMenu* menu, BHandler* handler);
	void message_received (BMessage* msg);
}

/**
 * The addon name. Will be used in menu item.
 */
const char* ADDON_NAME = "TrackGit";

enum {
	kClone,
	kInitHere
};

/**
 * process_ref definition for addon.
 * @param dir_ref The current directory ref.
 * @param msg BMessage containing refs to selected files.
 */
void 
process_refs (entry_ref dir_ref, BMessage* msg, void*)
{
	BString buffer("TrackGit: A Tracker Addon for Git Version Control System");
	BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
}

/**
 * Populates menu for this addon.
 * @param msg The passed BMessage. Contains refs to current dir and selected 
 * 			  files.
 * @param menu The pointer to Tracker menu.
 * @param handler The BHandler of Tracker. This should be target of added items.
 */
void
populate_menu (BMessage* msg, BMenu* menu, BHandler* handler)
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

	// Get all the selected refs
	vector<const char*> selected;
	BPath path;
	BEntry entry;
	int refs;
	entry_ref file_ref;
	for (refs=0;
			 msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR;
			 refs++) {
		entry.SetTo(&file_ref);
		entry.GetPath(&path);
		selected.push_back(path.Path());
	}
	
	// Get current directory path.
	entry_ref dir_ref;
	if (msg->FindRef("dir_ref", &dir_ref) != B_OK) {
		printf("No dir_ref found!\n");
		return;
	}
	entry.SetTo(&dir_ref);
	entry.GetPath(&path);
	const char* inPath = path.Path();

	git_libgit2_init();
	git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);

	// Check if current directory is in git repo.
	if (git_repository_discover(&buf, inPath, 0, NULL) == 0) {
		// buf is git repo
	} else {
		// inPath does not belong to git repo
		// Add Clone menu item
		BMessage* cloneMsg = new BMessage(*msg);
		cloneMsg->AddInt32("addon_item_id", kClone);
		BMenuItem* cloneItem = new BMenuItem(B_TRANSLATE("Clone"), cloneMsg);
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

	menu->AddItem(submenu);
	submenu->SetTargetForItems(handler);
}


/**
 * Handler for received messages.
 * @param msg The passed BMessage.
 */
void 
message_received (BMessage* msg)
{
	int32 itemId;
	if (msg->FindInt32("addon_item_id", &itemId) != B_OK)
		return;

	// Get all the selected refs
	BPath path;
	BEntry entry;
	vector<const char*> selected;
	int refs;
	entry_ref file_ref;
	for (refs=0;
			 msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR;
			 refs++) {
		entry.SetTo(&file_ref);
		entry.GetPath(&path);
		selected.push_back(path.Path());
	}
	
	// Get current directory path.
	entry_ref dir_ref;
	if (msg->FindRef("dir_ref", &dir_ref) != B_OK) {
		printf("No dir_ref found!\n");
		return;
	}
	entry.SetTo(&dir_ref);
	entry.GetPath(&path);

	GitCommand* gitCommand = NULL;

	switch (itemId) {
		case kClone:
			gitCommand = new Clone(path.Path());
			break;
		case kInitHere:
			gitCommand = new Init(path.Path());
			break;
		default:
			break;
	}

	if (gitCommand) {
		gitCommand->Execute();
	}
}


int 
main ()
{
	return 0;
}
