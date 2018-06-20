/**
 * @file PullWindow.h
 * @brief Header file of Pull window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _PULL_WINDOW_H_
#define _PULL_WINDOW_H_

#include "TrackGitWindow.h"

#include <git2.h>
#include <pthread.h>

#include <InterfaceKit.h>

class PullWindow : public TrackGitWindow
{
	/**
	 * The text view to show progress text.
	 */
	BTextView*				fTextView;
	/**
	 * Pull thread id.
	 */
	pthread_t				fThreadId;
	public:
							PullWindow(BString);
	void					SetText(const char*);
	virtual void			MessageReceived(BMessage*);
};

#endif

