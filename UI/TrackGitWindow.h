#ifndef _TRACKGIT_WINDOW_H_
#define _TRACKGIT_WINDOW_H_

#include "../Utils.h"

#include <AppKit.h>
#include <InterfaceKit.h>
#include <SupportKit.h>

class TrackGitWindow : public BWindow {
	BString fRepo;
public:
	TrackGitWindow(BString repo, BRect frame, const char* title, window_type type,
			int flags)
		:
		BWindow(frame, title, type, flags)
	{
		this->fRepo = repo;
	}
	virtual void Quit()
	{
		BMessenger messenger(APP_SIGN);
		BMessage msg(kQuitWindow);
		msg.AddString("repo", fRepo);
		messenger.SendMessage(&msg);
		BWindow::Quit();
	}
};

#endif
