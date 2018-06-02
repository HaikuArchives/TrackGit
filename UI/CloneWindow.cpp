#include "CloneWindow.h"

#include <stdio.h>

#include <Catalog.h>
#include <LayoutBuilder.h>
#include <SupportKit.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


enum {
	kDoClone,
	kCancel
};


CloneWindow::CloneWindow(BString repo, BString dirPath, Clone* clone)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 150), "TrackGit - Clone",
			B_DOCUMENT_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fClone = clone;
	fURL = new BTextControl(B_TRANSLATE("URL:"), "", NULL);
	fPathBox = new PathBox("pathbox", dirPath.String(), "Path:");

	BButton* fClone = new BButton("ok", B_TRANSLATE("Clone"),
			new BMessage(kDoClone));
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

	CenterOnScreen();
	Show();
}


void
CloneWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kDoClone:
			fClone->DoClone(fURL->Text(), fPathBox->Path());
			Quit();
			break;
		case kCancel:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
