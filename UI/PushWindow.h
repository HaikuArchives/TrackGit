/**
 * @file PushWindow.h
 * @brief Header file of Push window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _PUSH_WINDOW_H_
#define _PUSH_WINDOW_H_

#include "TrackGitWindow.h"

#include <git2.h>
#include <pthread.h>

#include <InterfaceKit.h>


/**
 * The Push window class.
 */
class PushWindow : public TrackGitWindow
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
							PushWindow(BString);
	void					SetText(const char*);
	virtual void			MessageReceived(BMessage*);
};

#endif

