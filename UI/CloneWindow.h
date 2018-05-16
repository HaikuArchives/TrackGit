#ifndef _CLONE_WINDOW_H_
#define _CLONE_WINDOW_H_

#include "../ThirdParty/PathBox.h"

#include <InterfaceKit.h>

class CloneWindow : public BWindow {
	BTextControl* fURL;
	PathBox* fPathBox;
	public:
	CloneWindow(const char*);
	virtual void MessageReceived(BMessage*);
	int DoClone(const char*, const char*);
};

#endif
