/**
 * @file Add.cpp
 * @brief Implementaion file of Add command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Add.h"

#include <InterfaceKit.h>

#include <stdlib.h>


/**
 * Add command constructor.
 * @param dirPath The directory where Init is selected.
 * @param files The files to be added.
 */
Add::Add(BString dirPath, vector<char*> files)
	:
	GitCommand()
{
	fDirPath = dirPath;
	fFiles = files;
}


/**
 * Initializes git_strarray.
 * @param files The files to be added to git_strarray.
 */
git_strarray
Add::InitArray(vector<char*> files)
{
	git_strarray array = {0};
	array.count = files.size();
	array.strings = (char**) malloc(sizeof(char*) * array.count);
	
	for (int i=0; i<array.count; i++) {
		array.strings[i] = files[i];
	}
	
	return array;
}

int print_matched_cb(const char* path, const char* matched_pathspec,
		void* payload)
{
	printf("path %s\n", path);
}


/**
 * Adds given files into git repository.
 * @param dirPath The given directory.
 * @param files The files to be added.
 */
int
Add::AddFiles(BString dirPath, vector<char*> files)
{
	git_index_matched_path_cb matched_cb = NULL;
	git_repository *repo = NULL;
	git_index *index;
	git_strarray array = {0};
	int options = 0, count = 0;
	struct print_payload payload;

	git_libgit2_init();

	array = InitArray(files);

	git_repository_open(&repo, dirPath.String());
	git_repository_index(&index, repo);

	payload.repo = repo;

	if (options&UPDATE) {
			git_index_update_all(index, &array, matched_cb, &payload);
	} else {
			git_index_add_all(index, &array, 0, matched_cb, &payload);
	}

	git_index_write(index);
	git_index_free(index);
	git_repository_free(repo);

	git_libgit2_shutdown();

	return 0;
}


/**
 * Add command excution. Adds given files into git repository.
 */
void
Add::Execute()
{
	if (AddFiles(fDirPath, fFiles) < 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return;
	}

	BString buffer("Files successfully added to the repository.");
	BAlert* alert = new BAlert("", buffer.String(), "OK", 
		0, 0, B_WIDTH_AS_USUAL);
	alert->Go();
}
