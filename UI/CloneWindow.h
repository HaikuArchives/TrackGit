#ifndef _CLONE_WINDOW_H_
#define _CLONE_WINDOW_H_

#include "../ThirdParty/PathBox.h"
#include "TrackGitWindow.h"

#include <git2.h>

#include <InterfaceKit.h>

typedef struct progress_data {
	git_transfer_progress fetch_progress;
	size_t completed_steps;
	size_t total_steps;
	const char *path;
} progress_data;


class CloneWindow : public TrackGitWindow {
	BAlert* progressAlert;
	BTextControl* fURL;
	PathBox* fPathBox;
	public:
	CloneWindow(const char*);
	virtual void MessageReceived(BMessage*);
	int DoClone(const char*, const char*);
	int FetchProgress(const char*, int, void*);
};

#endif
