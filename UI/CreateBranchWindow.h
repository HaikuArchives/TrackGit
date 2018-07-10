/**
 * @file CreateBranchWindow.h
 * @brief Header file of Create Branch window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CREATE_BRANCH_WINDOW_H_
#define _CREATE_BRANCH_WINDOW_H_

#include "TrackGitWindow.h"

#include <InterfaceKit.h>
#include <SupportKit.h>


enum {
	kDoCreateBranch,
	kCancelCreateBranch
};


/**
 * The Create Branch Window class.
 */
class CreateBranchWindow : public TrackGitWindow {
	/**
	 * The Create Branch Text View.
	 */
	BTextControl*		fBranchText;
public:
						CreateBranchWindow(BString);
	virtual void		MessageReceived(BMessage*);
};

#endif
