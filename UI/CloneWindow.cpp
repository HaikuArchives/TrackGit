#include "CloneWindow.h"

#include <stdio.h>

#include <Catalog.h>
#include <LayoutBuilder.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


enum {
	kClone,
	kCancel
};


CloneWindow::CloneWindow(char* dirPath, Clone* clone)
	:
	TrackGitWindow(BRect(0, 0, 300, 150), "TrackGit - Clone", B_DOCUMENT_WINDOW, 
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	this->clone = clone;
	fURL = new BTextControl(B_TRANSLATE("URL:"), "", NULL);
	fPathBox = new PathBox("pathbox", dirPath, "Path:");

	BButton* fClone = new BButton("ok", B_TRANSLATE("Clone"),
			new BMessage(kClone));
	BButton* fCancel = new BButton("ok", B_TRANSLATE("Cancel"),
			new BMessage(kCancel));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fURL)
		.Add(fPathBox)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.Add(fClone)
			.End();

	Show();
	CenterOnScreen();
}


void
CloneWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kClone:
			clone->DoClone(fURL->Text(), fPathBox->Path());
			Quit();
			break;
		case kCancel:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
