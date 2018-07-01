/**
 * @file StatusWindow.h
 * @brief Header file of Status window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _STATUS_WINDOW_H_
#define _STATUS_WINDOW_H_

#include "TrackGitWindow.h"

#include <InterfaceKit.h>
#include <SupportKit.h>


/**
 * The Status Window class.
 */
class StatusWindow : public TrackGitWindow {
	/**
	 * The Status Text View.
	 */
	BTextView* fStatusTextView;
public:
	StatusWindow(BString);
	void SetText(BString*);
	virtual void MessageReceived(BMessage*);
};

#endif
