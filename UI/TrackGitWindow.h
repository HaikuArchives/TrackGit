#ifndef _TRACKGIT_WINDOW_H_
#define _TRACKGIT_WINDOW_H_

#include <InterfaceKit.h>
#include <AppKit.h>

class TrackGitWindow : public BWindow {
public:
	TrackGitWindow(BRect frame, const char* title, window_type type,
			int flags)
		:
		BWindow(frame, title, type, flags)
	{}
	virtual bool QuitRequested()
	{
			be_app->PostMessage(B_QUIT_REQUESTED);
			return true;
	}
};

#endif
