/**
 * @file Commit.h
 * @brief Header file of Commit command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _COMMIT_H_
#define _COMMIT_H_

#include "GitCommand.h"

#include <SupportKit.h>

/**
 * Status command Class.
 */
class Commit : public GitCommand {
	/**
	 * The repo/directory where command is called.
	 */
	BString					fRepo;
	/**
	 * The Commit Window.
	 */
	TrackGitWindow*			fCommitWindow;
public:
							Commit(BString);

	virtual	void			Execute();
	virtual TrackGitWindow*	GetWindow();
	static int				DoCommit(BString, BString);
};

#endif
