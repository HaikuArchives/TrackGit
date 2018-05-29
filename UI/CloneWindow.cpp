#include "CloneWindow.h"

#include <stdio.h>

#include <Catalog.h>
#include <LayoutBuilder.h>

#define B_TRANSLATION_CONTEXT "TrackGit"


enum {
	kClone,
	kCancel
};


CloneWindow::CloneWindow(const char* dirPath)
	:
	TrackGitWindow(BRect(0, 0, 300, 150), "TrackGit - Clone", B_DOCUMENT_WINDOW, 
			B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
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


static void print_progress(const progress_data *pd)
{
	int network_percent = pd->fetch_progress.total_objects > 0 ?
			(100*pd->fetch_progress.received_objects) /
			pd->fetch_progress.total_objects :
			0;
	int index_percent = pd->fetch_progress.total_objects > 0 ?
			(100*pd->fetch_progress.indexed_objects) /
			pd->fetch_progress.total_objects :
			0;

	int checkout_percent = pd->total_steps > 0
			? (100 * pd->completed_steps) / pd->total_steps
			: 0;
	int kbytes = pd->fetch_progress.received_bytes / 1024;

	if (pd->fetch_progress.total_objects &&
			pd->fetch_progress.received_objects ==
			pd->fetch_progress.total_objects) {
			printf("Resolving deltas %d/%d\r",
				   pd->fetch_progress.indexed_deltas,
				   pd->fetch_progress.total_deltas);
	} else {
		printf("net %3d%% (%4d kb, %5d/%5d)  /  idx %3d%% (%5d/%5d)  /  chk %3d%% (%4d/%4d) %s\n",
			network_percent, kbytes,
			pd->fetch_progress.received_objects,
			pd->fetch_progress.total_objects,
			index_percent, pd->fetch_progress.indexed_objects,
			pd->fetch_progress.total_objects,
			checkout_percent,
			pd->completed_steps, pd->total_steps,
			pd->path);
	}
}


static int sideband_progress(const char *str, int len, void *payload)
{
	(void)payload; // unused

	printf("remote: %.*s\n", len, str);
	fflush(stdout);
	return 0;
}


static int fetch_progress(const git_transfer_progress *stats, void *payload)
{
	progress_data *pd = (progress_data*)payload;
	pd->fetch_progress = *stats;
	print_progress(pd);
	return 0;
}


static void checkout_progress(const char *path, size_t cur, size_t tot, void
		*payload)
{
	progress_data *pd = (progress_data*)payload;
	pd->completed_steps = cur;
	pd->total_steps = tot;
	pd->path = path;
	print_progress(pd);
}


int
CloneWindow::DoClone(const char* url, const char* path)
{
	printf("Cloning %s into %s\n", url, path);

	progress_data pd = {{0}};
	git_repository *cloned_repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
	int error;

	git_libgit2_init();

	// Set up options
	checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
	checkout_opts.progress_cb = checkout_progress;
	checkout_opts.progress_payload = &pd;
	clone_opts.checkout_opts = checkout_opts;
	clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
	clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;
	clone_opts.fetch_opts.callbacks.payload = &pd;

	// Do the clone
	int ret = git_clone(&cloned_repo, url, path, &clone_opts);
	
	if (ret != 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	}

	git_repository_free(cloned_repo);
	git_libgit2_shutdown();

	return ret;
}
