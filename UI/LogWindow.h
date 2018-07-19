/**
 * @file LogWindow.h
 * @brief Header file of Log window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _LOG_WINDOW_H_
#define _LOG_WINDOW_H_

#include "TrackGitWindow.h"

#include <InterfaceKit.h>
#include <SupportKit.h>


/**
 * The Log Window class.
 */
class LogWindow : public TrackGitWindow {
	/**
	 * The Log Text View.
	 */
	BTextView* fLogTextView;
public:
	LogWindow(BString);
	void SetText(BString);
	virtual void MessageReceived(BMessage*);
};

#endif
