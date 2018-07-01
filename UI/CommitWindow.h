/**
 * @file CommitWindow.h
 * @brief Header file of Commit window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _COMMIT_WINDOW_H_
#define _COMMIT_WINDOW_H_

#include "TrackGitWindow.h"

#include <InterfaceKit.h>
#include <SupportKit.h>


enum {
	kDoCommit,
	kCancelCommit
};


/**
 * The Commit Window class.
 */
class CommitWindow : public TrackGitWindow {
	/**
	 * The Commit Text View.
	 */
	BTextView*			fCommitTextView;
public:
						CommitWindow(BString);
	virtual void		MessageReceived(BMessage*);
};

#endif
