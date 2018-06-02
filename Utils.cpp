#include "Utils.h"

#include <StorageKit.h>

#include <git2.h>

#include <stdlib.h>


/**
 * Get selected files and current directory.
 * @param msg The BMessage containing refs to selected files and current
 *            directory.
 * @param selected The vector of selected file paths.
 * @returns The current directory path.
 */
void
extract_selected_paths(const BMessage* msg, vector<const char*>& selected)
{
	// Get all the selected refs
	entry_ref fileRef;
	for (int refs=0;
			 msg->FindRef("refs", refs, &fileRef) == B_NO_ERROR;
			 refs++) {
		BEntry entry;
		entry.SetTo(&fileRef);
		BPath path;
		entry.GetPath(&path);
		selected.push_back(path.Path());
	}
}

BString
extract_current_directory(const BMessage* msg)
{	
	// Get current directory path.
	entry_ref dirRef;
	if (msg->FindRef("dir_ref", &dirRef) != B_OK) {
		printf("No dir_ref found!\n");
		return NULL;
	}
	BEntry entry;
	entry.SetTo(&dirRef);
	BPath path;
	entry.GetPath(&path);
	return BString(path.Path());
}

BString
get_root_of_repo(BString dirPath)
{
	git_libgit2_init();
	git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);

	// Check if current directory is in git repo.
	int ret = git_repository_discover(&buf, dirPath.String(), 0, NULL);
	git_libgit2_shutdown();

	// Return root of repo if current directory is in git repo
	// else return current directory.
	return (ret == 0) ? BString(buf.ptr) : dirPath;
}
