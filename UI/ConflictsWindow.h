/**
 * @file ConflictsWindow.h
 * @brief Header file of Conflicts window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CONFLICTS_WINDOW_H_
#define _CONFLICTS_WINDOW_H_

#include <InterfaceKit.h>
#include <SupportKit.h>

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
