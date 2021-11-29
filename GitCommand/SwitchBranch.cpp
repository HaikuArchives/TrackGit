/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * Copyright 2021, Jaidyn Levesque <jadedctrl@teknik.io>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SwitchBranch.h"

#include <Alert.h>
#include <Catalog.h>
#include <Notification.h>
#include <StopWatch.h>
#include <StringList.h>

#include "../UI/SwitchBranchWindow.h"


#define B_TRANSLATION_CONTEXT "SwitchBranch"

/**
 * SwitchBranch class constructor.
 * @param repo The repo path where Switch Branch option is selected.
 */
SwitchBranch::SwitchBranch(BString repo, BMessage* msg)
{
	fRepo = repo;
	fSwitchBranchWindow = NULL;

	// If called by a branches individual menu item, save for Execute().
	if (msg != NULL)
		msg->FindString("branch", &fBranch);
}


/**
 * SwitchBranch command execution.
 */
void
SwitchBranch::Execute()
{
	if (fBranch.IsEmpty() == true)
		return;
	NotifyResult(DoSwitchBranch(fRepo, fBranch), fRepo, fBranch);
}


/**
 * This returns pointer to the Switch Branch window.
 * @returns The Switch Branch Window;
 */
TrackGitWindow*
SwitchBranch::GetWindow()
{
	if (fSwitchBranchWindow == NULL && fBranch.IsEmpty() == true)
		fSwitchBranchWindow = new SwitchBranchWindow(fRepo);
	return fSwitchBranchWindow;
}


/**
 * This function gets all the branche names in given repository.
 * @param repoPath The repo path.
 * @param branches The list of branch names.
 * @param current Name of the currently checked-out branch.
 * @param repo The git_repository, if libgit is already initialized.
 * @param cached Whether or not to use "cached" branches in the case of slow repos.
 * @returns 0 or error code
 */
int
SwitchBranch::GetBranches(BString repoPath, BStringList* branches,
	BString* current, git_repository* repo, bool cached)
{
	BStopWatch watch("GetBranches", true);
	git_branch_iterator* iterator;
	git_reference* ref;
	git_branch_t type;
	int ret = 0;

	if (repo == NULL) {
		git_libgit2_init();
		ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
		if (ret < 0)
			return ret;
	}

	BString cache;
	if (cached == true && _GetCachedBranches(repo, &cache, current) == B_OK) {
		if (branches != NULL)
			cache.Split(" ", true, *branches);
		return 0;
	}

	ret = git_branch_iterator_new(&iterator, repo, GIT_BRANCH_LOCAL);	
	if (ret < 0)
		return ret;

	while (git_branch_next(&ref, &type, iterator) == 0) {
		if (branches != NULL)
			branches->Add(git_reference_shorthand(ref));
		if (git_branch_is_checked_out(ref) == 1)
			if (current != NULL)
				current->SetTo(git_reference_shorthand(ref));
	}

	// If loading branches takes too long (over a second), cache them
	if (watch.ElapsedTime() > 999999)
		_CacheBranches(repo, branches, current);
	return ret;
}


/**
 * This function creates a new branch in repo with given name.
 * @param repoPath The repository path.
 * @param branchName The name of new branch.
 * @param repo The git_repository, if libgit is already initialized.
 * @returns 0 or error code.
 */
int
SwitchBranch::DoSwitchBranch(BString repoPath, BString branchName,
	git_repository* repo)
{
	git_object* tree = NULL;
	git_checkout_options opts;
	int ret = 0;

	if (repo == NULL) {
		git_libgit2_init();
		ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
		if (ret < 0)
			return ret;
	}
	
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

	if (ret == 0)
		_CacheBranches(repo, NULL, &branchName);

	git_repository_free(repo);

	git_libgit2_shutdown();

	return ret;
}


/**
 * Notifies the user of the command's result― BNotification for success, BAlert
 * for failure.
 * @param result The result of DoSwitchBranch().
 * @param repoPath The repository's .git path.
 * @param branch The attempted change of branch.
 */
void
SwitchBranch::NotifyResult(int result, const char* repoPath, const char* branch)
{
	if (result >= 0) {
		BNotification notify(B_INFORMATION_NOTIFICATION);
		notify.SetTitle(B_TRANSLATE("Branch switched"));
		if (branch != NULL) {
			BString desc = B_TRANSLATE("Current branch is %s.");
			desc.ReplaceAll("%s", branch);
			notify.SetContent(desc);
		}
		if (repoPath != NULL)
			notify.SetMessageID(BString(repoPath).Append("-Switch"));
		notify.Send();
	} else {
		const git_error* err = giterr_last();
		BString desc = B_TRANSLATE("Failed to switch branch.\nYou may want "
		"to manually run the `git checkout` command.\n\n%s");
		desc.ReplaceFirst("%s", err->message);

		BAlert* alert = new BAlert("", desc, B_TRANSLATE("Cancel"), 0, 0,
			B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	}
}


/**
 * Cache the given list of branches and/or checked-out branch
 * @param list Space-delimited list of the repo's branches
 * @param current The currently checked-out branch
 * @returns Any BNode attribute-writing status_t
 */
status_t
SwitchBranch::_CacheBranches(git_repository* repo, const BStringList* list,
	const BString* current)
{
	BNode gitDir(git_repository_workdir(repo));
	status_t ret = gitDir.InitCheck();
	if (ret != B_OK)
		return ret;

	time_t updated = time(NULL);
	gitDir.WriteAttr("Git:Cached", B_TIME_TYPE, 0, &updated, sizeof(time_t));

	if (current != NULL && current->IsEmpty() == false)
		ret = gitDir.WriteAttrString("Git:Branch", current);
	if (list != NULL && list->IsEmpty() == false) {
		BString cached = list->Join(" ");
		ret = gitDir.WriteAttrString("Git:Branches", &cached);
	}
	return ret;
}


/**
 * Retrieve cached data on branches, if any available
 * @param cached String to which a space-delimited list of branches is written
 * @param current String to which the currently checked-out branch is written
 * @returns Any BNode attribute-reading status_t, or B_ERROR for reset cache
 */
status_t
SwitchBranch::_GetCachedBranches(git_repository* repo, BString* cached,
	BString* current)
{
	BNode gitDir(git_repository_workdir(repo));
	status_t ret = gitDir.InitCheck();
	if (ret != B_OK)
		return ret;

	time_t updated = 0;
	gitDir.ReadAttr("Git:Cached", B_TIME_TYPE, 0, &updated, sizeof(time_t));
	if (time(NULL) - updated > (60 * 10)) {
		// Reset cached values every ~10 minutes
		gitDir.RemoveAttr("Git:Branch");
		gitDir.RemoveAttr("Git:Branches");
		gitDir.RemoveAttr("Git:Cached");
		return B_ERROR;
	}

	if (current != NULL)
		ret = gitDir.ReadAttrString("Git:Branch", current);
	if (ret != B_OK)
		return ret;
	if (cached != NULL)
		ret = gitDir.ReadAttrString("Git:Branches", cached);
	return ret;
}
