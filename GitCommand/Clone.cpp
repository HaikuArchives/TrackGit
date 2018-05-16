#include "Clone.h"
#include "../UI/CloneWindow.h"

#include <AppKit.h>

#include <git2.h>

Clone::Clone(const char* dirPath)
	:
	GitCommand()
{
	this->dirPath = dirPath;
}

void
Clone::Execute()
{
	CloneWindow* cloneWindow = new CloneWindow(dirPath);
	thread_id thread = cloneWindow->Thread();
	status_t win_status = B_OK;
	wait_for_thread(thread, &win_status);
}
