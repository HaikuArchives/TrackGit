/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _TRACKGIT_APP_H_
#define _TRACKGIT_APP_H_

#include <posix/sys/time.h>

#include <map>

#include <AppKit.h>
#include <SupportKit.h>

#if __GNUC__ > 2
	using std::map;
#endif	

class TrackGitWindow;


/**
 * The TrackGit Application class.
 * This takes care of launching of windows. It also brings forward a window
 * which is previously launched for a given repo.
 */
class TrackGitApp : public BApplication
{
public:
					TrackGitApp();

	virtual void	MessageReceived(BMessage*);
	virtual void	AboutRequested();

	static	void	AboutWindow();

private:
	/** The map of running commands to their repo. */
	map<BString, TrackGitWindow*> fRunningCommands;
};

#endif
