/**
 * @file CommitWindow.cpp
 * @brief Implementation file of Commit window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "CommitWindow.h"
#include "../GitCommand/Commit.h"

#include <stdio.h>
#include <git2.h>

#include <LayoutBuilder.h>


/**
 * StatusWindow Constructor
 * @param repo The repository to commit to.
 */
CommitWindow::CommitWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 200), "TrackGit - Commit",
			B_DOCUMENT_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	BStringView* fCommitString = new BStringView("commitString", 
			"Commit Message:");
	fCommitTextView = new BTextView("commitText");
	fCommitTextView->MakeFocus();
	
	BScrollView* fScrollView = new BScrollView("statusScrollView",
			fCommitTextView, B_WILL_DRAW | B_FRAME_EVENTS, false, true,
			B_PLAIN_BORDER);

	BButton* fCommit = new BButton("commit", "Commit",
			new BMessage(kDoCommit));
	BButton* fCancel = new BButton("cancel", "Cancel",
			new BMessage(kCancelCommit));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fCommitString)
		.Add(fScrollView)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.Add(fCommit)
			.End();

	CenterOnScreen();
	Show();
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
CommitWindow::MessageReceived(BMessage* msg)
{
	BAlert* alert = NULL;
	switch (msg->what) {
		case kDoCommit:
			if (Commit::DoCommit(fRepo, BString(fCommitTextView->Text()))) {
                const git_error* err = giterr_last();
                printf("Error %d : %s\n", err->klass, err->message);

                BString buffer("Error : %s");
                buffer.ReplaceFirst("%s", err->message);
                alert = new BAlert("", buffer.String(), "Cancel", 
                        0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			} else {
				BString buffer("Change commited successfully.");
                alert = new BAlert("", buffer.String(), "OK", 
                        0, 0, B_WIDTH_AS_USUAL);
			}
			alert->Go();
			Quit();
			break;
		case kCancelCommit:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
