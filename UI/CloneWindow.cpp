/**
 * @file CloneWindow.cpp
 * @brief Implementation file of Clone window.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "CloneWindow.h"

#include <stdio.h>
#include <git2.h>

#include <Catalog.h>
#include <LayoutBuilder.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


enum {
	kClone,
	kCancel
};

/**
 * CloneWindow Constructor
 * @param dirPath The path where Clone is selected.
 */
CloneWindow::CloneWindow(char* dirPath)
	:
	BWindow(BRect(0, 0, 300, 150), "TrackGit - Clone", B_DOCUMENT_WINDOW, 
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	printf("dir %s\n", dirPath);
	fURL = new BTextControl(B_TRANSLATE("URL:"), "", NULL);
	fPathBox = new PathBox("pathbox", dirPath, "Path:");

	BButton* fClone = new BButton("ok", B_TRANSLATE("Clone"),
			new BMessage(kClone));
	BButton* fCancel = new BButton("ok", B_TRANSLATE("Cancel"),
			new BMessage(kCancel));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(10)
		.Add(fURL)
		.Add(fPathBox)
		.AddGroup(B_HORIZONTAL, 0)
			.Add(fClone)
			.Add(fCancel)
			.AddGlue()
			.End();

	Show();
	CenterOnScreen();
}


/**
 * Handler to received messages.
 * @param msg The received message.
 */
void
CloneWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case kClone:
			DoClone(fURL->Text(), fPathBox->Path());
			Quit();
			break;
		case kCancel:
			Quit();
			printf("Cancel selected\n");
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}


/**
 * Clones given repo url into given path.
 * @param url The url of repo.
 * @param path The path to clone into.
 * @returns The error code or 0.
 */
int
CloneWindow::DoClone(const char* url, const char* path)
{
	printf("Cloning %s into %s\n", url, path);

	git_repository* repo = NULL;
	int ret = git_clone(&repo, url, path, NULL);
	
	if (ret != 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	}

	git_repository_free(repo);
	return ret;
}
