#ifndef _INIT_H_
#define _INIT_H_

#include "GitCommand.h"

class Init : public GitCommand
{
	const char* dirPath;
public:
	Init(const char*);
	virtual void Execute();
};

#endif
