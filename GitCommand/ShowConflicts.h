/**
 * @file ShowConflicts.h
 * @brief Header file of ShowConflicts command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _SHOW_CONFLICTS_H_
#define _SHOW_CONFLICTS_H_

#include "GitCommand.h"

#include <SupportKit.h>

/**
 * ShowConflicts command Class.
 */
class ShowConflicts : public GitCommand
{
	/**
	 * The repo path where ShowConflicts option is selected.
	 */
	BString fRepoPath;
public:
	ShowConflicts(BString);
	virtual void Execute();
};

#endif
