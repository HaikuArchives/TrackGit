/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "CreateBranchWindow.h"

#include <LayoutBuilder.h>

#include <git2.h>

#include "../GitCommand/CreateBranch.h"
#include "../GitCommand/SwitchBranch.h"


/**
 * CreateBranchWindow Constructor
 * @param repo The repository to commit to.
 */
CreateBranchWindow::CreateBranchWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 100), "TrackGit - Create Branch",
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE
			| B_AUTO_UPDATE_SIZE_LIMITS)
{
	fBranchText = new BTextControl("Name: ", "", NULL);
	fSwitchBranch = new BCheckBox("Switch to this branch");

	BButton* fCreate = new BButton("create", "Create",
			new BMessage(kDoCreateBranch));
	BButton* fCancel = new BButton("cancel", "Cancel",
			new BMessage(kCancelCreateBranch));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(fBranchText)
		.Add(fSwitchBranch)
		.AddGroup(B_HORIZONTAL)
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
		{
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
			if (fSwitchBranch->Value() == B_CONTROL_ON) {
				int res = SwitchBranch::DoSwitchBranch(fRepo, fBranchText->Text());
				SwitchBranch::NotifyResult(res, fRepo, fBranchText->Text());
			}
			Quit();
			break;
		}
		case kCancelCreateBranch:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
