/**
 * @file CloneWindow.h
 * @brief Header file of Clone window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CREDENTIALS_WINDOW_H_
#define _CREDENTIALS_WINDOW_H_

#include <InterfaceKit.h>

enum {
	kCredOK,
	kCredCancel
};

/**
 * The Credentials Window class.
 */
class CredentialsWindow : public BWindow {
	/**
	 * The text control for username.
	 */
	BTextControl* 			fUsername;
	/**
	 * The text control for password.
	 */
	BTextControl* 			fPassword;
	/**
	 * The username pointer.
	 */
	char*					fUsernamePtr;
	/**
	 * The password pointer.
	 */
	char*					fPasswordPtr;
	public:
							CredentialsWindow(char*, char*);
	virtual void			MessageReceived(BMessage*);
};

#endif
