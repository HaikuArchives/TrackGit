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

#if __GNUC__ > 2
	using std::vector;
#endif	


/**
 * Add command Class.
 */
class Add : public GitCommand
{
	/**
	 * The current directory where Init option is selected.
	 */
	BString				fDirPath;
	/**
	 * The list of files to be added.
	 */
	vector<char*>		fFiles;
public:
						Add(BString, vector<char*>);
	virtual void		Execute();
	static int			AddFiles(BString, vector<char*>);
	static git_strarray	InitArray(vector<char*> files);
	static void			FreeArray(git_strarray);
};

#endif
