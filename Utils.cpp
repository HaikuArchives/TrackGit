#include "Utils.h"

#include <StorageKit.h>

#include <stdlib.h>


/**
 * Get selected files and current directory.
 * @param msg The BMessage containing refs to selected files and current
 *            directory.
 * @param selected The vector of selected file paths.
 * @returns The current directory path.
 */
char*
get_selected(BMessage* msg, vector<const char*>* selected)
{
	// Get all the selected refs
	BPath path;
	BEntry entry;
	int refs;
	entry_ref file_ref;
	for (refs=0;
			 msg->FindRef("refs", refs, &file_ref) == B_NO_ERROR;
			 refs++) {
		entry.SetTo(&file_ref);
		entry.GetPath(&path);
		selected->push_back(path.Path());
	}
	
	// Get current directory path.
	entry_ref dir_ref;
	if (msg->FindRef("dir_ref", &dir_ref) != B_OK) {
		printf("No dir_ref found!\n");
		return NULL;
	}
	entry.SetTo(&dir_ref);
	entry.GetPath(&path);
	char* dirPath = (char*) malloc(strlen(path.Path()));
	strcpy(dirPath, path.Path());
	return dirPath;
}
