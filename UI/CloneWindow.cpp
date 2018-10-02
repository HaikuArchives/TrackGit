/**
 * @file CloneWindow.cpp
 * @brief Implementation file of Clone window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */
#include "CloneWindow.h"

#include <stdio.h>

#include <AppKit.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <SupportKit.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


/**
 * Constructor for CloneWindow.
 * @param repo The repo where command is called.
 * @param dirPath The current directory where clone optionn is selected.
 * @param clone The Clone command.
 */
CloneWindow::CloneWindow(BString repo, BString dirPath, Clone* clone)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 180), "TrackGit - Clone",
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
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


/**
 * Prints progress text to textview of window.
 * @param text The progress text.
 */
void
CloneWindow::SetProgressText(BString text)
{
	fProgressWindow->SetText(text.String());
}


/**
 * Sets the progress value of the progress bar in progress window.
 * @param progress The progress value.
 */
void
CloneWindow::SetProgress(float progress)
{
	fProgressWindow->SetProgress(progress);
}


/**
 * The handler to receive messages.
 * @param msg The message.
 */
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


/**
 * The CloneProgressWindow constructor.
 * @param cloneWindow The CloneWindow.
 */
CloneProgressWindow::CloneProgressWindow(CloneWindow* cloneWindow)
	:
	BWindow(BRect(0, 0, 300, 150), "TrackGit - Clone Progress",
			B_TITLED_WINDOW, B_NOT_CLOSABLE | B_NOT_RESIZABLE)
{
	fCloneWindow = cloneWindow;
	fTextView = new BTextView(BRect(0, 0, 280, 80), "_clone_", 
							  BRect(0, 0, 280, 80), B_FOLLOW_LEFT_RIGHT);
	fTextView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	fTextView->MakeEditable(false);
	fTextView->MakeSelectable(false);
	fTextView->SetWordWrap(true);
	fTextView->SetText("Cloning" B_UTF8_ELLIPSIS "\nRepository");

	fProgressBar = new BStatusBar("progressBar");
	fProgressBar->SetBarHeight(20);

	BButton* fCancel = new BButton("ok", "Cancel",
								  new BMessage(kCancel));

    BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fTextView)
		.Add(fProgressBar)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.End();
}


/**
 * This function sets texts of the textview within window.
 * @param text The text to be set.
 */
void
CloneProgressWindow::SetText(const char* text)
{
	if (LockLooper()) {
		fTextView->SetText(text);
		UnlockLooper();
	}
}


/**
 * This function sets the value of progress bar.
 * @param progress The progress value.
 */
void
CloneProgressWindow::SetProgress(float progress)
{
	if (LockLooper()) {
		fProgressBar->SetTo(progress);
		UnlockLooper();
	}
}


/**
 * The handler to receive messages.
 * @param msg The message received.
 */
void
CloneProgressWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kCancel:
			fCloneWindow->PostMessage(kCancel);
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
