/**
 * @file Status.h
 * @brief Header file of Status command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _STATUS_H_
#define _STATUS_H_

#include "GitCommand.h"
#include "../UI/StatusWindow.h"

#include <SupportKit.h>

#include <git2.h>

#define MAX_PATHSPEC 8

/**
 * Status Options structure
 */
struct opts {
  git_status_options statusopt;
  const char *repodir;
  char *pathspec[MAX_PATHSPEC];
  int npaths;
  int format;
  int zterm;
  int showbranch;
  int showsubmod;
  int repeat;
};


/**
 * Status command Class.
 */
class Status : public GitCommand {
	BString					fRepo;
	/**
	 * The current directory where Status option is selected.
	 */
	BString					fDirPath;
	/**
	 * The Status Window.
	 */
	StatusWindow* 			fStatusWindow;
public:
							Status(BString, BString);

	virtual	void			Execute();
	BString*				GetStatusText();
	virtual TrackGitWindow*	GetWindow();
	static BString*			GetStatusTextUtil(git_status_list*);
	static BString*			GetBranchText(git_repository*);
};

#endif
