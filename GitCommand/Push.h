/**
 * @file Push.h
 * @brief Header file of Push command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _PUSH_H_
#define _PUSH_H_

#include "GitCommand.h"
#include "../UI/PushWindow.h"

#include <SupportKit.h>

#include <pthread.h>
#include <git2.h>


/**
 * Parameters to pass to push thread.
 */
struct push_params {
	const char* path;
	PushWindow* pushWindow;
};


/**
 * Push command Class.
 */
class Push : public GitCommand
{
	/**
	 * The repository path where Push option is selected.
	 */
	BString					fRepo;
	/**
	 * Push Window.
	 */
	TrackGitWindow*			fPushWindow;
public:
							Push(BString);
	virtual void			Execute();
	virtual TrackGitWindow*	GetWindow();
	static pthread_t		DoPush(PushWindow*, const char*);
};

#endif
