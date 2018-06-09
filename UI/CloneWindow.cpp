#include "CloneWindow.h"

#include <stdio.h>

#include <AppKit.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <SupportKit.h>

#define B_TRANSLATION_CONTEXT "TrackGit"

CloneWindow::CloneWindow(BString repo, BString dirPath, Clone* clone)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 150), "TrackGit - Clone",
			B_DOCUMENT_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fClone = clone;
	fProgressWindow = new CloneProgressWindow(this);

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
CloneWindow::SetProgressText(BString text)
{
	fProgressWindow->SetText(text.String());
}


void
CloneWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kDoClone:
			fProgressWindow = new CloneProgressWindow(this);
			fProgressWindow->CenterOnScreen();
			fProgressWindow->Show();
			fThreadId = fClone->DoClone(this, fURL->Text(), fPathBox->Path());
			break;
		case kCancel:
			if (fThreadId)
				pthread_cancel(fThreadId);
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}

CloneProgressWindow::CloneProgressWindow(CloneWindow* cloneWindow)
	:
	BWindow(BRect(0, 0, 300, 150), "", B_MODAL_WINDOW,
			B_NOT_CLOSABLE | B_NOT_RESIZABLE)
{
	fCloneWindow = cloneWindow;
	fTextView = new BTextView("_clone_");
	fTextView->SetText("Cloning" B_UTF8_ELLIPSIS);
	fTextView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	fTextView->MakeEditable(false);
	fTextView->MakeSelectable(false);
	fTextView->SetWordWrap(true);
	fTextView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));
	BButton* fCancel = new BButton("ok", "Cancel",
								  new BMessage(kCancel));

    BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fTextView)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.End();
}

void
CloneProgressWindow::SetText(const char* text)
{
	if (LockLooper()) {
		fTextView->SetText(text);
		UnlockLooper();
	}
}

void
CloneProgressWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kCancel:
			BInvoker(new BMessage(kCancel), NULL, fCloneWindow).Invoke();
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
