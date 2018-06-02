/**
 * @file GitCommand.h
 * @brief Header file of GitCommand. The base class to all git commands.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _GIT_COMMAND_H_
#define _GIT_COMMAND_H_

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
		return 0;
	}
};

#endif
