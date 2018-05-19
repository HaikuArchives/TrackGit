/**
 * @file Clone.h
 * @brief Header file of Clone command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#ifndef _CLONE_H_
#define _CLONE_H_

#include "GitCommand.h"

/**
 * Clone command Class.
 */
class Clone : public GitCommand {
	/**
	 * The current directory where Clone option is selected.
	 */
	const char*				dirPath;
public:
							Clone(const char*);

	virtual	void			Execute();
};

#endif
