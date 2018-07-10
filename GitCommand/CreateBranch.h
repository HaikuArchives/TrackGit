/**
 * @file CreateBranch.h
 * @brief Header file of Create Branch command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CREATE_BRANCH_H_
#define _CREATE_BRANCH_H_

#include "GitCommand.h"

#include <SupportKit.h>

/**
 * Create Branch command Class.
 */
class CreateBranch : public GitCommand {
	/**
	 * The repo/directory where command is called.
	 */
	BString					fRepo;
	/**
	 * The Create Branch Window.
	 */
	TrackGitWindow*			fCreateBranchWindow;
public:
							CreateBranch(BString);

	virtual	void			Execute();
	virtual TrackGitWindow*	GetWindow();
	static int				DoCreateBranch(BString, BString);
};

#endif
