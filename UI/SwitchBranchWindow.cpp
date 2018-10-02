/**
 * @file SwitchBranchWindow.cpp
 * @brief Implementation file of Switch Branch window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "SwitchBranchWindow.h"
#include "../GitCommand/SwitchBranch.h"

#include <stdio.h>
#include <git2.h>

#include <LayoutBuilder.h>

#include <vector>


/**
 * SwitchBranchWindow Constructor
 * @param repo The repository to commit to.
 */
SwitchBranchWindow::SwitchBranchWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 100), "TrackGit - Switch Branch",
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	vector<BString> branches;

	if (SwitchBranch::GetBranches(fRepo, branches) != 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);
		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		Quit();
	}

	fSwitchMenu = new BPopUpMenu(branches[0]);
	fSelectedBranch = branches[0];

	BButton* fSwitch = new BButton("switch", "Switch",
			new BMessage(kDoSwitchBranch));
	BButton* fCancel = new BButton("cancel", "Cancel",
			new BMessage(kCancelSwitchBranch));

	for (int i=0; i<branches.size(); i++) {
		BMessage *msg = new BMessage(kBranchName);
		msg->AddString("branch", branches[i]);
		fSwitchMenu->AddItem(new BMenuItem(branches[i].String(), msg));
	}

	BMenuField* switchMenuField = new BMenuField("Branch: ", fSwitchMenu);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(switchMenuField)
		.AddGroup(B_HORIZONTAL, 0)
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
			if (SwitchBranch::DoSwitchBranch(fRepo, fSelectedBranch) < 0) {
                const git_error* err = giterr_last();

                BString buffer("Error : %s");
				buffer.ReplaceFirst("%s", err->message);
                alert = new BAlert("", buffer.String(), "Cancel", 
                        0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			} else {
				BString buffer("Switched to branch %b.");
				buffer.ReplaceFirst("%b", fSelectedBranch.String());
                alert = new BAlert("", buffer.String(), "OK", 
                        0, 0, B_WIDTH_AS_USUAL);
			}
			alert->Go();
			Quit();
			break;
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
