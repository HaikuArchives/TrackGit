/**
 * @file TrackGitWindow.h
 * @brief Header file of TrackGit window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _TRACKGIT_WINDOW_H_
#define _TRACKGIT_WINDOW_H_

#include "../Utils.h"

#include <AppKit.h>
#include <InterfaceKit.h>
#include <SupportKit.h>

class TrackGitWindow : public BWindow {
protected:
	/**
	 * The repo/directory where the command is called.
	 */
	BString fRepo;
public:
	/**
	 * The TrackGitWindow constructor.
	 * @param repo The repo/directory where command is called.
	 * @param title The title of Window.
	 * @param type The window type.
	 * @param flags Any additional flags.
	 */
	TrackGitWindow(BString repo, BRect frame, const char* title, window_type type,
			int flags)
		:
		BWindow(frame, title, type, flags)
	{
		fRepo = repo;
	}
	/**
	 * The function to Quit the window.
	 * This function sends a BMessage to TrackGit app to clear this window from
	 * it's map.
	 */ 
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
