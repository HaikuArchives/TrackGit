/**
 * @file Utils.cpp
 * @brief Implementation file of Utils.
 *		  This file contains common functions and enums used by app.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Utils.h"

#include <StorageKit.h>

#include <git2.h>

#include <stdlib.h>
#include <string.h>


/**
 * Get current directory.
 * @param msg The BMessage containing ref to current directory.
 * @return The current directory.
 */
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


/**
 * Get selected files.
 * @param msg The BMessage containing refs to selected files.
 * @param selected The vector of selected file paths.
 */
void
extract_selected_paths(const BMessage* msg, vector<char*>& selected)
{
	// Get current directory
	BString dir = extract_current_directory(msg);
	// Get all the selected refs
	entry_ref fileRef;
	for (int refs=0;
			 msg->FindRef("refs", refs, &fileRef) == B_NO_ERROR;
			 refs++) {
		BEntry entry;
		entry.SetTo(&fileRef);
		BPath path;
		entry.GetPath(&path);
		BString relativePath(path.Path());
		// Generate relative path from absolute path
		relativePath.ReplaceFirst(dir, "");
		relativePath.ReplaceFirst("/", "");
		// To convert const char* to char*
		char* p = (char*) malloc(relativePath.Length());
		strcpy(p, relativePath.String());
		selected.push_back(p);
	}
}


/**
 * Get the root of repo of current directory.
 * @param dirPath The current directory.
 * @returns The repo root if found else current directory.
 */
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
