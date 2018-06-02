#ifndef _CLONE_WINDOW_H_
#define _CLONE_WINDOW_H_

#include "../ThirdParty/PathBox.h"
#include "TrackGitWindow.h"
#include "../GitCommand/Clone.h"

#include <git2.h>

#include <InterfaceKit.h>

class CloneWindow : public TrackGitWindow {
	BAlert* progressAlert;
	BTextControl* fURL;
	PathBox* fPathBox;
	Clone* clone;
	public:
	CloneWindow(BString, BString, Clone*);
	virtual void MessageReceived(BMessage*);
};

#endif
