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
	kStatusOK,
	kStatusSelected
};


/**
 * StatusItem Constructor.
 * @param text The text to be displayed.
 * @param path The path of the file.
 */
StatusItem::StatusItem(BString text, BString path)
	:
	BStringItem(text)
{
	fText = text;
	fPath = path;
}


/**
 * Getter for path.
 * @returns The path of the file.
 */
BString
StatusItem::GetPath()
{
	return fPath;
}


/**
 * StatusWindow Constructor
 * @param repo The repo to get status of.
 */
StatusWindow::StatusWindow(BString repo)
	:
	TrackGitWindow(repo, BRect(0, 0, 300, 300), "TrackGit - Status",
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fStatusListView = new BListView(B_SINGLE_SELECTION_LIST);
	//fStatusListView->SetSelectionMessage(new BMessage(kStatusSelected));
	fStatusListView->SetInvocationMessage(new BMessage(kStatusSelected));
	
	BScrollView* fScrollView = new BScrollView("statusScrollView",
			fStatusListView, B_WILL_DRAW | B_FRAME_EVENTS, false, true,
			B_PLAIN_BORDER);

	BButton* fOK = new BButton("ok", B_TRANSLATE("OK"),
			new BMessage(kStatusOK));

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(fScrollView)
		.Add(fOK);

	CenterOnScreen();
	Show();
}


/**
 * Adds item to the Status list.
 * @param text The text to be set.
 * @param path The path of the file.
 */
void
StatusWindow::AddItem(BString text, BString path)
{
	if (LockLooper()) {
		fStatusListView->AddItem(new StatusItem(text, path));
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
	int selection;
	StatusItem* item;
	BString path;
	BEntry entry;
	entry_ref ref;
	BMessenger msgr;
	BMessage rmsg;
	switch (msg->what) {
		case kStatusSelected:
			selection = fStatusListView->CurrentSelection();
			item = (StatusItem*)fStatusListView->ItemAt(selection);
			if (item->GetPath().Length() > 0) {
				path = fRepo;
				path.ReplaceFirst(".git/", "");
				path.Append(item->GetPath());
				printf("Selected %s\n", path.String());
				entry = BEntry(path.String());
				entry.GetRef(&ref);
				msgr = BMessenger("application/x-vnd.Be-TRAK");
				rmsg = BMessage(B_REFS_RECEIVED);
				rmsg.AddRef("refs", &ref);
				msgr.SendMessage(&rmsg);
			}
			break;
		case kStatusOK:
			Quit();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
