#ifndef _CLONE_WINDOW_H_
#define _CLONE_WINDOW_H_

#include "../ThirdParty/PathBox.h"
#include "TrackGitWindow.h"
#include "../GitCommand/Clone.h"

#include <git2.h>
#include <pthread.h>

#include <InterfaceKit.h>

enum {
	kDoClone,
	kCancel
};

class CloneProgressWindow;

class CloneWindow : public TrackGitWindow {
	CloneProgressWindow* 	fProgressWindow;
	BTextControl* 			fURL;
	PathBox* 				fPathBox;
	Clone* 					fClone;
	pthread_t 				fThreadId;
	public:
							CloneWindow(BString, BString, Clone*);
	void					SetProgressText(BString);
	virtual void			MessageReceived(BMessage*);
};

class CloneProgressWindow : public BWindow
{
	CloneWindow*		fCloneWindow;
	BTextView*			fTextView;
	public:
						CloneProgressWindow(CloneWindow*);
	void				SetText(const char*);
	virtual void		MessageReceived(BMessage*);
};

#endif
