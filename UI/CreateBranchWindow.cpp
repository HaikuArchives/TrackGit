/**
 * @file CreateBranchWindow.cpp
 * @brief Implementation file of Create Branch window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "CreateBranchWindow.h"
#include "../GitCommand/CreateBranch.h"

#include <stdio.h>
#include <git2.h>

#include <LayoutBuilder.h>


/**
 * CreateBranchWindow Constructor
 * @param repo The repository to commit to.
 */
CreateBranchWindow::CreateBranchWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 100), "TrackGit - Create Branch",
			B_DOCUMENT_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fBranchText = new BTextControl("Name: ", "", NULL);

	BButton* fCreate = new BButton("create", "Create",
			new BMessage(kDoCreateBranch));
	BButton* fCancel = new BButton("cancel", "Cancel",
			new BMessage(kCancelCreateBranch));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fBranchText)
		.AddGroup(B_HORIZONTAL, 0)
			.AddGlue()
			.Add(fCancel)
			.Add(fCreate)
			.End();

	CenterOnScreen();
	Show();
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
CreateBranchWindow::MessageReceived(BMessage* msg)
{
	BAlert* alert = NULL;
	switch (msg->what) {
		case kDoCreateBranch:
			if (CreateBranch::DoCreateBranch(fRepo,
						BString(fBranchText->Text())) < 0) {
                const git_error* err = giterr_last();

                BString buffer("Error : %s");
				if (err)
					buffer.ReplaceFirst("%s", err->message);
				else
					buffer.ReplaceFirst("%s", "Invalid branch name. Does the "
							"branch already exists?");
                alert = new BAlert("", buffer.String(), "Cancel", 
                        0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			} else {
				BString buffer("Branch %b created successfully.");
				buffer.ReplaceFirst("%b", fBranchText->Text());
                alert = new BAlert("", buffer.String(), "OK", 
                        0, 0, B_WIDTH_AS_USUAL);
			}
			alert->Go();
			Quit();
			break;
		case kCancelCreateBranch:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
