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
	
	for (int i=0; i<array.count; i++)
		array.strings[i] = files[i];
	
	return array;
}


/**
 * Adds given files into git repository.
 * @param dirPath The given directory.
 * @param files The files to be added.
 */
int
Add::AddFiles(BString dirPath, vector<char*> files)
{
	git_repository *repo = NULL;
	git_index *index;
	git_strarray array = InitArray(files);
	int ret;

	git_libgit2_init();

	ret = git_repository_open(&repo, dirPath.String());
	if (ret != 0)
		return ret;

	ret = git_repository_index(&index, repo);
	if (ret != 0)
		return ret;

	ret = git_index_add_all(index, &array, 0, NULL, NULL);
	if (ret != 0)
		return ret;

	ret = git_index_write(index);
	if (ret != 0)
		return ret;

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
