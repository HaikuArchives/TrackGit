/**
 * @file CloneWindow.h
 * @brief Header file of Clone window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */#

ifndef _CLONE_WINDOW_H_
#define _CLONE_WINDOW_H_

#include "../ThirdParty/PathBox.h"

#include <InterfaceKit.h>

class CloneWindow : public BWindow {
	/**
	 * URL Text Box
	 */
	BTextControl* fURL;
	/**
	 * Path Text Box
	 */ 
	PathBox* fPathBox;
	
public:
	CloneWindow(const char*);
	virtual void MessageReceived(BMessage*);
	int DoClone(const char*, const char*);
};

#endif
