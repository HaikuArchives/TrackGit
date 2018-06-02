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
	CloneWindow(char*, Clone*);
	virtual void MessageReceived(BMessage*);
	int DoClone(const char*, const char*);
	int FetchProgress(const char*, int, void*);
};

#endif
