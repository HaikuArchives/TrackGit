/**
 * @file Log.h
 * @brief Header file of Log command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _LOG_H_
#define _LOG_H_

#include "GitCommand.h"
#include "../UI/LogWindow.h"

#include <SupportKit.h>

#include <git2.h>

/**
 * log_state represents walker being configured while handling options
 */
struct log_state {
	git_repository *repo;
	const char *repodir;
	git_revwalk *walker;
	int hide;
	int sorting;
	int revisions;
};


/**
 * log_options holds other command line options that affect log output
 */
struct log_options {
        int show_diff;
        int show_log_size;
        int skip, limit;
        int min_parents, max_parents;
        git_time_t before;
        git_time_t after;
        const char *author;
        const char *committer;
        const char *grep;
};


/**
 * Log command Class.
 */
class Log : public GitCommand {
	/**
	 * The repo/directory where command is called.
	 */
	BString					fRepo;
	/**
	 * The Log Window.
	 */
	LogWindow*	 			fLogWindow;
public:
							Log(BString);

	virtual	void			Execute();
	BString					GetLogText();
	virtual TrackGitWindow*	GetWindow();
};

#endif
