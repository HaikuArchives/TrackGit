/**
 * @file Pull.h
 * @brief Header file of Pull command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _PULL_H_
#define _PULL_H_

#include "GitCommand.h"
#include "../UI/PullWindow.h"

#include <SupportKit.h>

#include <pthread.h>
#include <git2.h>


/**
 * Payload to search for merge branch.
 */
struct fetch_payload {
	char branch[100];
	git_oid branch_oid;
};

/**
 * Parameters to pass to pull thread.
 */
struct pull_params {
	const char* path;
	PullWindow* pullWindow;
};


/**
 * Pull command Class.
 */
class Pull : public GitCommand
{
	/**
	 * The repository path where Pull option is selected.
	 */
	BString					fRepo;
	/**
	 * Pull Window.
	 */
	TrackGitWindow*			fPullWindow;
public:
							Pull(BString);
	virtual void			Execute();
	virtual TrackGitWindow*	GetWindow();
	static pthread_t		DoPull(PullWindow*, const char*);
};

#endif
