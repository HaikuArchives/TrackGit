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
#include <pthread.h>

class CloneWindow;

typedef struct progress_data {
	git_transfer_progress fetch_progress;
	size_t completed_steps;
	size_t total_steps;
	const char *path;
	CloneWindow* cloneWindow;
} progress_data;


struct param {
	const char* url;
	const char* path;
	CloneWindow* cloneWindow;
};


/**
 * Clone command Class.
 */
class Clone : public GitCommand {
	BString					fRepo;
	/**
	 * The current directory where Clone option is selected.
	 */
	BString					fDirPath;
	TrackGitWindow*			fCloneWindow;
public:
							Clone(BString, BString);
	TrackGitWindow*			GetWindow();
	virtual	void			Execute();
	pthread_t				DoClone(CloneWindow*, const char*, const char*);
};

#endif
