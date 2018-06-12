/**
 * @file Add.h
 * @brief Header file of Add command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _ADD_H_
#define _ADD_H_

#include "GitCommand.h"

#include <SupportKit.h>

#include <vector>

#include <git2.h>


enum print_options {
	SKIP = 1,
	VERBOSE = 2,
	UPDATE = 4
};

/**
 * Payload for Add git command.
 */
struct print_payload {
	enum print_options options;
	git_repository* repo;
};


/**
 * Init command Class.
 */
class Add : public GitCommand
{
	/**
	 * The current directory where Init option is selected.
	 */
	BString fDirPath;
	/**
	 * The list of files to be added.
	 */
	vector<char*> fFiles;
public:
	Add(BString, vector<char*>);
	virtual void Execute();
	static int AddFiles(BString, vector<char*>);
	static git_strarray InitArray(vector<char*> files);
};

#endif
