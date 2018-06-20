/**
 * @file ConflictsWindow.cpp
 * @brief Implementation file of Conflicts window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "ConflictsWindow.h"

#include <stdio.h>

#include <LayoutBuilder.h>

enum {
	kOK
};

/**
 * ConflictsWindow Constructor
 */
ConflictsWindow::ConflictsWindow()
	:
	BWindow(BRect(0, 0, 300, 300), "TrackGit - Conflicts",
			B_DOCUMENT_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fConflictsTextView = new BTextView("ConflictsText");
	fConflictsTextView->SetText("Loading...");
	fConflictsTextView->MakeEditable(false);
	
	BScrollView* fScrollView = new BScrollView("ConflictsScrollView",
			fConflictsTextView, B_WILL_DRAW | B_FRAME_EVENTS, false, true,
			B_PLAIN_BORDER);

	BButton* fOK = new BButton("ok", "OK", new BMessage(kOK));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fScrollView)
		.Add(fOK);

	CenterOnScreen();
	Show();
}


/**
 * Sets Text of the View in Window.
 * @param text The text to be set.
 */
void
ConflictsWindow::SetText(BString text)
{
	if (LockLooper()) {
		fConflictsTextView->SetText(text.String());
		UnlockLooper();
	}
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
ConflictsWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kOK:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
