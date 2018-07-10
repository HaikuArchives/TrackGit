/**
 * @file SwitchBranchWindow.h
 * @brief Header file of Switch Branch window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _SWITCH_BRANCH_WINDOW_H_
#define _SWITCH_BRANCH_WINDOW_H_

#include "TrackGitWindow.h"

#include <InterfaceKit.h>
#include <SupportKit.h>

enum {
	kDoSwitchBranch,
	kCancelSwitchBranch,
	kBranchName
};


/**
 * The Switch Branch Window class.
 */
class SwitchBranchWindow : public TrackGitWindow {
	/**
	 * The Switch Branch pop up menu.
	 */
	BPopUpMenu*			fSwitchMenu;
	/**
	 * The branch to be switched to.
	 */
	BString				fSelectedBranch;
public:
						SwitchBranchWindow(BString);
	virtual void		MessageReceived(BMessage*);
};

#endif
