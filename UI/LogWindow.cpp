/**
 * @file LogWindow.cpp
 * @brief Implementation file of Log window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "LogWindow.h"

#include <stdio.h>

#include <Catalog.h>
#include <LayoutBuilder.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


enum {
	kOK
};


/**
 * LogWindow Constructor
 * @param repo The repo to get log of.
 */
LogWindow::LogWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 400, 300), "TrackGit - Log",
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fLogTextView = new BTextView("logText");
	fLogTextView->SetText("Loading...");
	fLogTextView->MakeEditable(false);
	
	BScrollView* fScrollView = new BScrollView("logScrollView",
			fLogTextView, B_WILL_DRAW | B_FRAME_EVENTS, false, true,
			B_PLAIN_BORDER);

	BButton* fOK = new BButton("ok", B_TRANSLATE("OK"),
			new BMessage(kOK));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
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
LogWindow::SetText(BString text)
{
	if (LockLooper()) {
		fLogTextView->SetText(text.String());
		UnlockLooper();
	}
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
LogWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kOK:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
