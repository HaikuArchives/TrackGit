/**
 * @file Status.h
 * @brief Header file of Status command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _STATUS_H_
#define _STATUS_H_

#include "GitCommand.h"

#include <git2.h>

#define MAX_PATHSPEC 8

/**
 * Status Options structure
 */
struct opts {
  git_status_options statusopt;
  char *repodir;
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
	/**
	 * The current directory where Status option is selected.
	 */
	char*				dirPath;
public:
							Status(char*);

	virtual	void			Execute();
};

#endif
