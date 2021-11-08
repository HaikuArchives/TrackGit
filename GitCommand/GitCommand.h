/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _GIT_COMMAND_H_
#define _GIT_COMMAND_H_

#include <stddef.h>

#include <posix/sys/time.h>

class TrackGitWindow;

/**
 * GitCommand Class.
 * This class is abstract class which are inherited by various Git commands.
 * These commands are the interface to libgit2.
 */
class GitCommand {
    public:
    /**
     * This is where actual calls to libgit2 will go.
     */
    virtual void Execute() = 0;
	/**
	 * This is used to get pointer to the window (if any).
	 * @returns The pointer to the window of command.
	 */
	virtual TrackGitWindow* GetWindow()
	{
		return NULL;
	}
};

#endif
