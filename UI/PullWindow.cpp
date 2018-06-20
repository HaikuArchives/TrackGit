/**
 * @file PullWindow.cpp
 * @brief Implementation file of Pull window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */
#include "PullWindow.h"
#include "../GitCommand/Pull.h"

#include <stdio.h>

#include <AppKit.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <SupportKit.h>

enum {
	kCancelPull
};

/**
 * The PullWindow constructor.
 * @param cloneWindow The CloneWindow.
 */
PullWindow::PullWindow(BString repo)
        :
        TrackGitWindow(repo, BRect(0, 0, 300, 150), "", B_MODAL_WINDOW,
                        B_NOT_CLOSABLE | B_NOT_RESIZABLE)
{
	fTextView = new BTextView(BRect(0, 0, 280, 80), "_clone_", 
						  BRect(0, 0, 280, 80), B_FOLLOW_LEFT_RIGHT);
	fTextView->SetViewUIColor(B_PANEL_BACKGROUND_COLOR);
	fTextView->MakeEditable(false);
	fTextView->MakeSelectable(false);
	fTextView->SetWordWrap(true);
	fTextView->SetText("Pulling" B_UTF8_ELLIPSIS "\nRepository");
	BButton* fCancel = new BButton("ok", "Cancel",
								  new BMessage(kCancelPull));

    BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fTextView)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.End();

	CenterOnScreen();
	Show();
	fThreadId = Pull::DoPull(this, repo.String());
}


/**
 * This function sets texts of the textview within window.
 * @param text The text to be set.
 */
void
PullWindow::SetText(const char* text)
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
PullWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kCancelPull:
			if (fThreadId)
				pthread_cancel(fThreadId);
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}

