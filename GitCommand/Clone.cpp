#include "Clone.h"
#include "../UI/CloneWindow.h"

#include <git2.h>

Clone::Clone()
	:
	GitCommand()
{
}

void
Clone::Execute()
{
	CloneWindow* cloneWindow = new CloneWindow();
	cloneWindow->Show();
}
