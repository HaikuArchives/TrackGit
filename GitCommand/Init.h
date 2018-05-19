/**
 * @file Init.h
 * @brief Header file of Init command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _INIT_H_
#define _INIT_H_

#include "GitCommand.h"

/**
 * Init command Class.
 */
class Init : public GitCommand
{
	/**
	 * The current directory where Init option is selected.
	 */
	const char* dirPath;
public:
	Init(const char*);
	virtual void Execute();
};

#endif
