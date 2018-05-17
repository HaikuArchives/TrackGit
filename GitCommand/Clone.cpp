#include "Clone.h"
#include "../UI/CloneWindow.h"

#include <AppKit.h>

#include <git2.h>

/**
 * Clone class Constructor.
 * @param dirPath The current directory where Clone is selected.
 */
Clone::Clone(const char* dirPath)
	:
	GitCommand()
{
	this->dirPath = dirPath;
}

/**
 * Clone command execution. Opens a window to clone a repo.
 */
void
Clone::Execute()
{
	CloneWindow* cloneWindow = new CloneWindow(dirPath);
	thread_id thread = cloneWindow->Thread();
	status_t win_status = B_OK;
	wait_for_thread(thread, &win_status);
}
