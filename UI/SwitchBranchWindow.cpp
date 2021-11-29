/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SwitchBranchWindow.h"

#include <Catalog.h>
#include <LayoutBuilder.h>
#include <StringList.h>

#include <git2.h>

#include "../GitCommand/SwitchBranch.h"


#define B_TRANSLATION_CONTEXT "SwitchBranchWindow"

/**
 * SwitchBranchWindow Constructor
 * @param repo The repository to commit to.
 */
SwitchBranchWindow::SwitchBranchWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 100),
			B_TRANSLATE("TrackGit - Switch Branch"), B_TITLED_WINDOW,
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	BStringList branches;

	if (SwitchBranch::GetBranches(fRepo, &branches, NULL, NULL, false) != 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);
		BString buffer(B_TRANSLATE("Failed to retrieve branches.\nYou may want "
			"to manually run the `git branch` and `git checkout` commands.\n\n%s"));
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), B_TRANSLATE("Cancel"), 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		Quit();
	}

	fSwitchMenu = new BPopUpMenu(branches.StringAt(0));
	fSelectedBranch = branches.StringAt(0);

	BButton* fSwitch = new BButton("switch", B_TRANSLATE("Switch"),
			new BMessage(kDoSwitchBranch));
	BButton* fCancel = new BButton("cancel", B_TRANSLATE("Cancel"),
			new BMessage(kCancelSwitchBranch));

	for (int i = 0; i < branches.CountStrings(); i++) {
		BMessage *msg = new BMessage(kBranchName);
		msg->AddString("branch", branches.StringAt(i));
		fSwitchMenu->AddItem(new BMenuItem(branches.StringAt(i), msg));
	}

	BMenuField* switchMenuField = new BMenuField(B_TRANSLATE("Branch: "),
		fSwitchMenu);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(switchMenuField)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fCancel)
			.Add(fSwitch)
			.End();

	CenterOnScreen();
	Show();
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
SwitchBranchWindow::MessageReceived(BMessage* msg)
{
	BAlert* alert = NULL;
	BString branchName;
	switch (msg->what) {
		case kDoSwitchBranch:
		{
			int result = SwitchBranch::DoSwitchBranch(fRepo, fSelectedBranch);
			SwitchBranch::NotifyResult(result, fRepo, fSelectedBranch);
			if (result == 0)
				Quit();
			break;
		}
		case kCancelSwitchBranch:
			Quit();
			break;
		case kBranchName:
			if (msg->FindString("branch", &branchName) == B_OK)
				fSelectedBranch = branchName;
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
