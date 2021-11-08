/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "LogWindow.h"

#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Screen.h>

#define B_TRANSLATION_CONTEXT "Log Window"


/**
 * LogWindow Constructor
 * @param repo The repo to get log of.
 */
LogWindow::LogWindow(BString repo)
	:
	TrackGitWindow(repo, window_rect_by_text_column(80),
			B_TRANSLATE("TrackGit - Log"),
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	fLogTextView = new BTextView("logText");
	fLogTextView->SetText(B_TRANSLATE("Loading" B_UTF8_ELLIPSIS));
	fLogTextView->MakeEditable(false);

	fLoading = true;
	
	BScrollView* fScrollView = new BScrollView("logScrollView",
			fLogTextView, B_WILL_DRAW | B_FRAME_EVENTS, false, true,
			B_PLAIN_BORDER);

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(fScrollView);

	CenterOnScreen();
	Show();
}


/**
 * Appends text to the View in Window.
 * @param text Text to be appended
 */
void
LogWindow::AppendText(BString text)
{
	if (LockLooper()) {
		if (fLoading == true) {
			fLogTextView->SetText(text);
			fLoading = false;
		}
		else
			fLogTextView->Insert(fLogTextView->TextLength(), text.String(), text.Length());
		UnlockLooper();
	}
}


BRect
window_rect_by_text_column(int32 columns)
{
	int32 width = (be_plain_font->StringWidth("x") * columns) + 10;
	int32 height = 0.9 * width;

	BRect screen = BScreen().Frame();
	if (screen.Width() < width || screen.Height() < height)
		return BRect(0, 0, 400, 300);
	else
		return BRect(0, 0, width, height);
}
