/**
 * @file Commit.cpp
 * @brief Implementation of Commit command.
 *
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Commit.h"
#include "../UI/CommitWindow.h"
#include "../Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <git2.h>


/**
 * Commit class constructor.
 * @param repo The repo path where commit option is selected.
 */
Commit::Commit(BString repo)
{
	fRepo = repo;
	fCommitWindow = NULL;
}


/**
 * Commit command execution.
 */
void
Commit::Execute()
{
}


/**
 * This returns pointer to the commit window.
 * @returns The Commit Window;
 */
TrackGitWindow*
Commit::GetWindow()
{
	if (fCommitWindow == NULL)
		fCommitWindow = new CommitWindow(fRepo);
	return NULL;
}


/**
 * This function commits to given repo with given message.
 * @param repoPath The repository path.
 * @param message The commit message.
 * @returns 0 or error code.
 */
int
Commit::DoCommit(BString repoPath, BString message)
{
	git_repository* repo;
	git_index* index;
	int ret = 0;

	git_libgit2_init();

	// Init repo
	ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
	if (ret < 0)
		return ret;

	// Init index
	ret = git_repository_index(&index, repo);
	if (ret < 0)
		goto ret;

	ret = create_commit(repo, index, message.String());
ret:
	git_repository_free(repo);

	git_libgit2_shutdown();

	return ret;
}
