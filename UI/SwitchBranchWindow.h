/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _SWITCH_BRANCH_WINDOW_H_
#define _SWITCH_BRANCH_WINDOW_H_

#include <InterfaceKit.h>
#include <SupportKit.h>

#include "TrackGitWindow.h"


enum {
	kDoSwitchBranch,
	kCancelSwitchBranch,
	kBranchName
};


class SwitchBranchWindow : public TrackGitWindow {
public:
						SwitchBranchWindow(BString);
	virtual void		MessageReceived(BMessage*);

private:
	BPopUpMenu*			fSwitchMenu;
	BString				fSelectedBranch;
};

#endif
