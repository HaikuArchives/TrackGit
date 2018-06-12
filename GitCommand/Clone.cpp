/**
 * @file Clone.cpp
 * @brief Implemention file of Clone command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Clone.h"
#include "../UI/CloneWindow.h"

#include <AppKit.h>

#include <stdlib.h>
#include <strings.h>

#include <git2.h>

/**
 * Clone class Constructor.
 * @para repo The repo where the command is called.
 * @param dirPath The current directory where Clone is selected.
 */
Clone::Clone(BString repo, BString dirPath)
	:
	GitCommand()
{
	fRepo = repo;
	fDirPath = dirPath;
	fCloneWindow = NULL;
}


/**
 * This returns pointer to the clone window.
 * @returns The Clone Window;
 */
TrackGitWindow*
Clone::GetWindow()
{
	if (fCloneWindow == NULL)
		fCloneWindow = new CloneWindow(fRepo, fDirPath, this);
	return fCloneWindow;
}


/**
 * Clone command execution. Opens a window to clone a repo.
 */
void
Clone::Execute()
{
}


/**
 * Prints progress of command to the progress window.
 * @param progress The progress data.
 */
static void
print_progress(const progress_data* progress)
{
	int network_percent = progress->fetch_progress.total_objects > 0 ?
			(100*progress->fetch_progress.received_objects) /
			progress->fetch_progress.total_objects :
			0;
	int index_percent = progress->fetch_progress.total_objects > 0 ?
			(100*progress->fetch_progress.indexed_objects) /
			progress->fetch_progress.total_objects :
			0;

	int checkout_percent = progress->total_steps > 0
			? (100 * progress->completed_steps) / progress->total_steps
			: 0;
	int kbytes = progress->fetch_progress.received_bytes / 1024;
	
	BString progressString;

	if (progress->fetch_progress.total_objects &&
			progress->fetch_progress.received_objects ==
			progress->fetch_progress.total_objects) {
		progressString << "Resolving deltas " 
			<< progress->fetch_progress.indexed_deltas
			<< "/" << progress->fetch_progress.total_deltas;
	} else {
		progressString << "Network " << network_percent 
			<< " (" << kbytes << " kb, "
			<< progress->fetch_progress.received_objects << "/"
			<< progress->fetch_progress.total_objects << ")\n";
		progressString << "Indexed " << index_percent << " (" 
			<< progress->fetch_progress.indexed_objects << "/"
			<< progress->fetch_progress.total_objects << ")\n";
	}

	if (progress->cloneWindow)
		progress->cloneWindow->SetProgressText(progressString);
}


/**
 * Prints the progress from remote.
 * @param str The remote progress.
 */
static int
sideband_progress(const char *str, int len, void *payload)
{
	(void)payload; // unused

	printf("remote: %.*s\n", len, str);
	fflush(stdout);
	return 0;
}


/**
 * Creates data structures for printing progress.
 * @param stats The fetch progress.
 * @param payload The progress payload.
 */
static int
fetch_progress(const git_transfer_progress *stats, void *payload)
{
	progress_data *progress = (progress_data*)payload;
	progress->fetch_progress = *stats;
	print_progress(progress);
	return 0;
}


/**
 * Prints checkout progress.
 * @param path The clone path.
 * @param cur Current completed steps.
 * @param tot Total steps.
 * @param payload The progress payload.
 */
static void
checkout_progress(const char *path, size_t cur, size_t tot, void
		*payload)
{
	progress_data *progress = (progress_data*)payload;
	progress->completed_steps = cur;
	progress->total_steps = tot;
	progress->path = path;
	print_progress(progress);
}


/**
 * Clones the given url into given path along with showing progress in 
 * given progress window.
 * @param arg This contains url, path and progress window.
 */
void*
DoCloneThread(void* arg)
{
	struct param* p = (struct param*) arg;
	const char* url = p->url;
	const char* path = p->path;

	printf("Cloning %s into %s\n", url, path);
	BString cloneText;
	cloneText << "Cloning " << url << " into " << path;
	if (p->cloneWindow)
		p->cloneWindow->SetProgressText(cloneText);

	progress_data progress = {{0}};
	progress.cloneWindow = p->cloneWindow;
	git_repository *cloned_repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
	int error;

	git_libgit2_init();

	// Set up options
	checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
	checkout_opts.progress_cb = checkout_progress;
	checkout_opts.progress_payload = &progress;
	clone_opts.checkout_opts = checkout_opts;
	clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
	clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;
	clone_opts.fetch_opts.callbacks.payload = &progress;

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
	if (p->cloneWindow && p->cloneWindow->LockLooper())
		p->cloneWindow->Quit();
	free(arg);
}


/**
 * Spawns a thread to clone.
 * @param cloneWindow Window to show progress.
 * @param url The url to clone.
 * @param path The path to clone to.
 */
pthread_t
Clone::DoClone(CloneWindow* cloneWindow, const char* url, const char* path)
{
	struct param *p = (struct param*) malloc(sizeof(struct param));
	p->url = url;
	p->path = path;
	p->cloneWindow = cloneWindow;
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, DoCloneThread, p);
	return thread_id;
}
