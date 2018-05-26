/**
 * @file StatusWindow.cpp
 * @brief Implementation file of Status window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "StatusWindow.h"

#include <stdio.h>

#include <Catalog.h>
#include <LayoutBuilder.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


enum {
	kOK
};

/**
 * StatusWindow Constructor
 * @param statusText The status text to be displayed.
 */
StatusWindow::StatusWindow()
	:
	BWindow(BRect(0, 0, 300, 300), "TrackGit - Status", B_DOCUMENT_WINDOW, 
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fStatusTextView = new BTextView("statusText");
	fStatusTextView->SetText("Loading...");
	fStatusTextView->MakeEditable(false);
	
	BScrollView* fScrollView = new BScrollView("statusScrollView",
			fStatusTextView, B_WILL_DRAW | B_FRAME_EVENTS, false, true,
			B_PLAIN_BORDER);

	BButton* fOK = new BButton("ok", B_TRANSLATE("OK"),
			new BMessage(kOK));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fScrollView)
		.Add(fOK);

	Show();
	CenterOnScreen();
}


/**
 * Sets Text of the View in Window.
 * @param text The text to be set.
 */
void
StatusWindow::SetText(BString* text)
{
	if (LockLooper()) {
		fStatusTextView->SetText(text->String());
		UnlockLooper();
	}
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
StatusWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kOK:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
