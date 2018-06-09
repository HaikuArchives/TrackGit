/**
 * @file TrackGitApp.h
 * @brief Header file of TrackGit App.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _TRACKGIT_APP_H_
#define _TRACKGIT_APP_H_

#include <map>

#include <AppKit.h>
#include <SupportKit.h>

class TrackGitWindow;

class TrackGitApp : public BApplication
{
	/**
	 * The map of running commands to their repo.
	 */
	map<BString, TrackGitWindow*> fRunningCommands;
	public:
	TrackGitApp();
	virtual void MessageReceived(BMessage*);
};

#endif
