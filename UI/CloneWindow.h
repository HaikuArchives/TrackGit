#ifndef _CLONE_WINDOW_H_
#define _CLONE_WINDOW_H_

#include <InterfaceKit.h>

class CloneWindow : public BWindow {
	public:
	CloneWindow();
	virtual void MessageReceived(BMessage*);
};

#endif
