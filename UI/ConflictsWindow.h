/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _CONFLICTS_WINDOW_H_
#define _CONFLICTS_WINDOW_H_

#include <posix/sys/time.h>

#include <InterfaceKit.h>
#include <SupportKit.h>


/**
 * The Conflicts Window class.
 */
class ConflictsWindow : public BWindow {
	/**
	 * The Conflicts Text View.
	 */
	BTextView* fConflictsTextView;
public:
	ConflictsWindow();
	void SetText(BString);
	virtual void MessageReceived(BMessage*);
};

#endif
