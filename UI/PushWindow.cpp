/**
 * @file PushWindow.cpp
 * @brief Implementation file of Push window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */
#include "PushWindow.h"
#include "../GitCommand/Push.h"

#include <stdio.h>

#include <AppKit.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <SupportKit.h>

enum {
	kCancelPush
};

/**
 * The PushWindow constructor.
 * @param cloneWindow The CloneWindow.
 */
PushWindow::PushWindow(BString repo)
        :
        TrackGitWindow(repo, BRect(0, 0, 300, 150), "TrackGit - Push",
						B_DOCUMENT_WINDOW, B_NOT_CLOSABLE | B_NOT_RESIZABLE)
{
	fTextView = new BTextView(BRect(0, 0, 280, 80), "_clone_", 
						  BRect(0, 0, 280, 80), B_FOLLOW_LEFT_RIGHT);
	fTextView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	fTextView->MakeEditable(false);
	fTextView->MakeSelectable(false);
	fTextView->SetWordWrap(true);
	fTextView->SetText("Pushing" B_UTF8_ELLIPSIS "\nRepository");
	BButton* fCancel = new BButton("ok", "Cancel",
								  new BMessage(kCancelPush));

    BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fTextView)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.End();

	CenterOnScreen();
	Show();
	fThreadId = Push::DoPush(this, repo.String());
}


/**
 * This function sets texts of the textview within window.
 * @param text The text to be set.
 */
void
PushWindow::SetText(const char* text)
{
	if (LockLooper()) {
		fTextView->SetText(text);
		UnlockLooper();
	}
}


/**
 * The handler to receive messages.
 * @param msg The message received.
 */
void
PushWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kCancelPush:
			if (fThreadId)
				pthread_cancel(fThreadId);
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}

