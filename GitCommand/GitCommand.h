#ifndef _GIT_COMMAND_H_
#define _GIT_COMMAND_H_

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
};

#endif
