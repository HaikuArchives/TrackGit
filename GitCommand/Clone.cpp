/**
 * @file Clone.cpp
 * @brief Implemention file of Clone command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Clone.h"
#include "../UI/CloneWindow.h"

#include <AppKit.h>

#include <stdlib.h>
#include <strings.h>

#include <git2.h>

/**
 * Clone class Constructor.
 * @param dirPath The current directory where Clone is selected.
 */
Clone::Clone(const char* dirPath)
	:
	GitCommand()
{
	this->dirPath = (char*) malloc(strlen(dirPath));
	strcpy(this->dirPath, dirPath);
}


/**
 * This returns pointer to the clone window. It is used by TrackGitApp to
 * check if window is already present.
 * @returns The Clone Window;
 */
TrackGitWindow*
Clone::GetWindow()
{
	TrackGitWindow* cloneWindow = new CloneWindow(dirPath);
	return cloneWindow;
}


/**
 * Clone command execution. Opens a window to clone a repo.
 */
void
Clone::Execute()
{
}
