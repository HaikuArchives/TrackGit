#ifndef _CLONE_H_
#define _CLONE_H_

#include "GitCommand.h"

class Clone : public GitCommand {
public:
							Clone();

	virtual	void			Execute();
};

#endif
