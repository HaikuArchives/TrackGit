/**
 * @file Init.h
 * @brief Header file of Init command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _INIT_H_
#define _INIT_H_

#include "GitCommand.h"

#include <SupportKit.h>

/**
 * Init command Class.
 */
class Init : public GitCommand
{
	/**
	 * The current directory where Init option is selected.
	 */
	BString fDirPath;
public:
	Init(BString);
	virtual void Execute();
	static int InitRepo(BString);
};

#endif
