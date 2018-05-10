#include <Alert.h>
#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <SupportKit.h>
#include <MenuItem.h>

#include <stdio.h>
#include <strings.h>
#include <add-ons/tracker/TrackerAddOn.h>

#include <git2.h>

extern "C" {
	void populate_menu (BMessage* msg, BMenu* menu, BHandler* handler);
	void message_received (BMessage* msg);
}

const char* ADDON_NAME = "TrackGit";
const uint32 kClone = 'clon';

void 
process_refs (entry_ref dir_ref, BMessage* msg, void*)
{
	BString buffer("Hello World");
	BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
}


void
populate_menu (BMessage* msg, BMenu* menu, BHandler* handler)
{
	printf("in TrackGit::populate_menu");
	if (menu == NULL){
		BString buffer("Null menu");
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
				0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return;
	}

	BMenuItem* item = menu->FindItem(ADDON_NAME);
	if (item != NULL)
		menu->RemoveItem(item);

	BMenu* submenu = new BMenu(ADDON_NAME);

	// Init libgit2
	git_libgit2_init();

	entry_ref dir_ref;
	if (msg->FindRef("dir_ref", &dir_ref) != B_OK) {
		printf("No dir_ref found!\n");
		return;
	}
	BPath path;
	BEntry entry(&dir_ref);
	entry.GetPath(&path);
	const char* inPath = path.Path();
	git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);

	if (git_repository_discover(&buf, inPath, 0, NULL) == 0) {
		// repoPath is git repo
	} else {
		// inPath does not belong to git repo
		BMessage* itemMsg = new BMessage(*msg);
		itemMsg->AddInt32("addon_item_id", kClone);
		BMenuItem *menuitem = new BMenuItem("Clone", itemMsg);
		submenu->AddItem(menuitem);
	}

	menu->AddItem(submenu);
	submenu->SetTargetForItems(handler);
}


void 
message_received (BMessage* msg)
{
	int32 itemId;
	if (msg->FindInt32("addon_item_id", &itemId) != B_OK)
		return;

	switch (itemId) {
		case kClone:
			break;
		default:
			break;
	}
}


int 
main ()
{
	return 0;
}
