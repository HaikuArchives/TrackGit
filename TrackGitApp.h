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
	/**
	 * The map of running commands to their repo.
	 */
	map<BString, TrackGitWindow*> fRunningCommands;
	public:
	TrackGitApp();
	virtual void MessageReceived(BMessage*);
};

#endif
