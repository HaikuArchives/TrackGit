#include "CloneWindow.h"

#include <Catalog.h>
#include <LayoutBuilder.h>

#define B_TRANSLATION_CONTEXT "TrackGit"

CloneWindow::CloneWindow()
	:
	BWindow(BRect(0, 0, 100, 100), "Clone", B_DOCUMENT_WINDOW, 
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	BTextControl* fURL = new BTextControl(B_TRANSLATE("URL"), "", NULL);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0)
		.AddGroup(B_VERTICAL, 0)
			.Add(fURL)
		.End();
}

void
CloneWindow::MessageReceived(BMessage*)
{
}
