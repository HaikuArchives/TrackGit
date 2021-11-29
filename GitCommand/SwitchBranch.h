/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * Copyright 2021, Jaidyn Levesque <jadedctrl@teknik.io>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _SWITCH_BRANCH_H_
#define _SWITCH_BRANCH_H_

#include <SupportKit.h>

#include <git2.h>

#include "GitCommand.h"


class SwitchBranch : public GitCommand {
public:
							SwitchBranch(BString repo, BMessage* msg = NULL);

	virtual	void			Execute();
	virtual TrackGitWindow*	GetWindow();

	static int				DoSwitchBranch(BString repoPath, BString branchName,
								git_repository* repo = NULL);
	static int				GetBranches(BString repoPath, BStringList* branches,
								BString* current = NULL,
								git_repository* repo = NULL,
								bool cached = true);

	static void				NotifyResult(int result, const char* repoPath,
								const char* branch);

private:
	static status_t			_CacheBranches(git_repository* repo,
								const BStringList* list, const BString* current);
	static status_t			_GetCachedBranches(git_repository* repo,
								BString* cached, BString* current);

	BString					fRepo;
	BString					fBranch;
	TrackGitWindow*			fSwitchBranchWindow;
};

#endif
