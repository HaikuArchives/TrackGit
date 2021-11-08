/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _LOG_WINDOW_H_
#define _LOG_WINDOW_H_

#include <InterfaceKit.h>
#include <SupportKit.h>

#include "TrackGitWindow.h"


/**
 * The Log Window class.
 */
class LogWindow : public TrackGitWindow {
public:
					LogWindow(BString);
			void	AppendText(BString);

private:
	bool fLoading;
	BTextView* fLogTextView;
};


BRect window_rect_by_text_column(int32 columns);

#endif
