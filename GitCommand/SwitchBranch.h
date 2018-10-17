/**
 * @file SwitchBranch.h
 * @brief Header file of Switch Branch command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _SWITCH_BRANCH_H_
#define _SWITCH_BRANCH_H_

#include "GitCommand.h"

#include <SupportKit.h>

#include <vector>

#if __GNUC__ > 2
	using std::vector;
#endif	


/**
 * Switch Branch command Class.
 */
class SwitchBranch : public GitCommand {
	/**
	 * The repo/directory where command is called.
	 */
	BString					fRepo;
	/**
	 * The Switch Branch Window.
	 */
	TrackGitWindow*			fSwitchBranchWindow;
public:
							SwitchBranch(BString);

	virtual	void			Execute();
	virtual TrackGitWindow*	GetWindow();
	static int				DoSwitchBranch(BString, BString);
	static int				GetBranches(BString, vector<BString>&);
};

#endif
