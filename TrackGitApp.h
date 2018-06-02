#ifndef _TRACKGIT_APP_H_
#define _TRACKGIT_APP_H_

#include "UI/TrackGitWindow.h"

#include <map>

#include <AppKit.h>

class TrackGitApp : public BApplication
{
	map<int, TrackGitWindow*> windowMap;
	public:
	TrackGitApp();
	virtual void MessageReceived(BMessage*);
};

#endif
