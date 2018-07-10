/**
 * @file CreateBranch.cpp
 * @brief Implementation of Create Branch command.
 *
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "CreateBranch.h"
#include "../UI/CreateBranchWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <git2.h>


/**
 * CreateBranch class constructor.
 * @param repo The repo path where Create Branch option is selected.
 */
CreateBranch::CreateBranch(BString repo)
{
	fRepo = repo;
	fCreateBranchWindow = NULL;
}


/**
 * CreateBranch command execution.
 */
void
CreateBranch::Execute()
{
}


/**
 * This returns pointer to the Create Branch window.
 * @returns The Create Branch Window;
 */
TrackGitWindow*
CreateBranch::GetWindow()
{
	if (fCreateBranchWindow == NULL)
		fCreateBranchWindow = new CreateBranchWindow(fRepo);
	return NULL;
}


/**
 * This function creates a new branch in repo with given name.
 * @param repoPath The repository path.
 * @param branchName The name of new branch.
 * @returns 0 or error code.
 */
int
CreateBranch::DoCreateBranch(BString repoPath, BString branchName)
{
	git_repository* repo;
	git_reference *head, *branch;
	git_commit* commit;
	git_oid commit_oid;
	const char *currentBranch = NULL;
	int ret = 0;

	git_libgit2_init();

	ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
	if (ret < 0)
		return ret;

	ret = git_repository_head(&head, repo);
	if (ret < 0)
		return ret;

	currentBranch = git_reference_shorthand(head);

	ret = git_reference_name_to_id(&commit_oid, repo, "HEAD");
	if (ret < 0)
		return ret;

	ret = git_commit_lookup(&commit, repo, &commit_oid);
	if (ret < 0)
		return ret;

	ret = git_branch_create(&branch, repo, branchName.String(), commit, 0);
ret:
	git_repository_free(repo);

	git_libgit2_shutdown();

	return ret;
}
