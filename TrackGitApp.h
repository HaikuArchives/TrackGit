#ifndef _TRACKGIT_APP_H_
#define _TRACKGIT_APP_H_

#include <map>

#include <AppKit.h>
#include <SupportKit.h>

class TrackGitWindow;

class TrackGitApp : public BApplication
{
	map<BString, TrackGitWindow*> fRunningCommands;
	public:
	TrackGitApp();
	virtual void MessageReceived(BMessage*);
};

#endif
