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
 * The Status List Item class.
 */
class StatusItem : public BStringItem {
	/**
	 * The text to be displayed.
	 */
	BString				fText;
	/**
	 * The file path of item.
	 */
	BString				fPath;
public:
						StatusItem(BString, BString);
	BString				GetPath();
};


/**
 * The Status Window class.
 */
class StatusWindow : public TrackGitWindow {
	/**
	 * The Status List View.
	 */
	BListView*			fStatusListView;
public:
						StatusWindow(BString);
	void				AddItem(BString, BString);
	virtual void		MessageReceived(BMessage*);
};

#endif
