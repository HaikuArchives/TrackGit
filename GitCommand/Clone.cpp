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
}


/**
 * This returns pointer to the clone window.
 * @returns The Clone Window;
 */
TrackGitWindow*
Clone::GetWindow()
{
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
 * @param pd The progress data.
 */
static void
print_progress(const progress_data *pd)
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
	
	BString progress;

	if (pd->fetch_progress.total_objects &&
			pd->fetch_progress.received_objects ==
			pd->fetch_progress.total_objects) {
		progress << "Resolving deltas " << pd->fetch_progress.indexed_deltas
				<< "/" << pd->fetch_progress.total_deltas;
	} else {
		progress << "Network " << network_percent << " (" << kbytes << " kb, "
			<< pd->fetch_progress.received_objects << "/"
			<< pd->fetch_progress.total_objects << ")\n";
		progress << "Indexed " << index_percent << " (" 
			<< pd->fetch_progress.indexed_objects << "/"
			<< pd->fetch_progress.total_objects << ")\n";
	}

	if (pd->cloneWindow)
		pd->cloneWindow->SetProgressText(progress);
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
	progress_data *pd = (progress_data*)payload;
	pd->fetch_progress = *stats;
	print_progress(pd);
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
	progress_data *pd = (progress_data*)payload;
	pd->completed_steps = cur;
	pd->total_steps = tot;
	pd->path = path;
	print_progress(pd);
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

	progress_data pd = {{0}};
	pd.cloneWindow = p->cloneWindow;
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
	if (p->cloneWindow && p->cloneWindow->LockLooper())
		p->cloneWindow->Quit();
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
