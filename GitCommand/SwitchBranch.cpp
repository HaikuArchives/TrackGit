/**
 * @file SwitchBranch.cpp
 * @brief Implementation of Switch Branch command.
 *
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "SwitchBranch.h"
#include "../UI/SwitchBranchWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <git2.h>

/**
 * SwitchBranch class constructor.
 * @param repo The repo path where Switch Branch option is selected.
 */
SwitchBranch::SwitchBranch(BString repo)
{
	fRepo = repo;
	fSwitchBranchWindow = NULL;
}


/**
 * SwitchBranch command execution.
 */
void
SwitchBranch::Execute()
{
}


/**
 * This function gets all the branche names in given repository.
 * @param repoPath The repo path.
 * @param branches The list of branch names.
 * @returns 0 or error code
 */
int
SwitchBranch::GetBranches(BString repoPath, vector<BString>& branches)
{
	git_repository* repo;
	git_branch_iterator* iterator;
	git_reference* ref;
	git_branch_t type;
	int ret = 0;

	git_libgit2_init();

	ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
	if (ret < 0)
		return ret;

	ret = git_branch_iterator_new(&iterator, repo, GIT_BRANCH_LOCAL);	
	if (ret < 0)
		return ret;

	while (git_branch_next(&ref, &type, iterator) == 0)
		branches.push_back(git_reference_shorthand(ref));

	return ret;
}


/**
 * This returns pointer to the Switch Branch window.
 * @returns The Switch Branch Window;
 */
TrackGitWindow*
SwitchBranch::GetWindow()
{
	if (fSwitchBranchWindow == NULL)
		fSwitchBranchWindow = new SwitchBranchWindow(fRepo);
	return fSwitchBranchWindow;
}


/**
 * This function creates a new branch in repo with given name.
 * @param repoPath The repository path.
 * @param branchName The name of new branch.
 * @returns 0 or error code.
 */
int
SwitchBranch::DoSwitchBranch(BString repoPath, BString branchName)
{
	git_repository* repo;
	git_object* tree = NULL;
	git_checkout_options opts;
	int ret = 0;

	git_libgit2_init();

	ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
	if (ret < 0)
		return ret;
	
	git_checkout_init_options(&opts, GIT_CHECKOUT_OPTIONS_VERSION);

	ret = git_revparse_single(&tree, repo, branchName.String());
	if (ret < 0)
		return ret;

	ret = git_checkout_tree(repo, tree, &opts);
	if (ret < 0)
		return ret;
	
	BString ref("refs/heads/%s");
	ref.ReplaceFirst("%s", branchName.String());
	ret = git_repository_set_head(repo, ref.String());
ret:
	git_repository_free(repo);

	git_libgit2_shutdown();

	return ret;
}
