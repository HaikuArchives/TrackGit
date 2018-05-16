#ifndef _CLONE_H_
#define _CLONE_H_

#include "GitCommand.h"

class Clone : public GitCommand {
public:
							Clone(const char*);

	virtual	void			Execute();
private:
	const char*				dirPath;
};

#endif
