/**
 * @file ShowConflicts.cpp
 * @brief Implementaion file of ShowConflicts command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "ShowConflicts.h"
#include "../Utils.h"

#include <InterfaceKit.h>

#include <git2.h>


/**
 * ShowConflicts command constructor.
 * @param repoPath The repository path where ShowConflicts is selected.
 */
ShowConflicts::ShowConflicts(BString repoPath)
	:
	GitCommand()
{
	fRepoPath = repoPath;
}


/**
 * ShowConflicts command excution.
 * Opens up a window showing list of files which are conflicting.
 */
void
ShowConflicts::Execute()
{
	git_repository* repo;
	git_index* index;
	int ret = 0;

	// Init repo
	ret = git_repository_open_ext(&repo, fRepoPath.String(), 0, NULL);
	if (ret < 0)
		goto ret;

	// Init index
	ret = git_repository_index(&index, repo);
	if (ret < 0)
		goto ret;

	output_conflicts(index);
ret:
	if (ret < 0) {
		const git_error* er = giterr_last();
		printf("Error %d : %s\n", er->klass, er->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", er->message);
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
				0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	}
	git_repository_free(repo);
	git_libgit2_shutdown();
}
