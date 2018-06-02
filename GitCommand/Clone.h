/**
 * @file Clone.h
 * @brief Header file of Clone command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CLONE_H_
#define _CLONE_H_

#include "GitCommand.h"

#include <SupportKit.h>

#include <git2.h>

typedef struct progress_data {
	git_transfer_progress fetch_progress;
	size_t completed_steps;
	size_t total_steps;
	const char *path;
} progress_data;


/**
 * Clone command Class.
 */
class Clone : public GitCommand {
	BString					fRepo;
	/**
	 * The current directory where Clone option is selected.
	 */
	BString					fDirPath;
public:
							Clone(BString, BString);
	TrackGitWindow*			GetWindow();
	virtual	void			Execute();
	int						DoClone(const char*, const char*);
};

#endif
