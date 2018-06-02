#ifndef _UTILS_H_
#define _UTILS_H_

#include <AppKit.h>

#include <vector.h>

enum {
	kClone,
	kInitHere,
	kStatus
};


/**
 * The addon name. Will be used in menu item.
 */
#define ADDON_NAME "TrackGit"

/**
 * The Application signature.
 */
#define APP_SIGN "application/x-vnd.Haiku-TrackGit"

char* get_selected(BMessage* msg, vector<const char*>* selected);

#endif
