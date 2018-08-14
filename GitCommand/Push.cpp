/**
 * @file Push.cpp
 * @brief Implementaion file of Push command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Push.h"
#include "../Utils.h"

#include <InterfaceKit.h>

#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Push command constructor.
 * @param dirPath The directory where Init is selected.
 */
Push::Push(BString repo)
	:
	GitCommand()
{
	fRepo = repo;
	fPushWindow = NULL;
}


void
Push::Execute()
{
}


/**
 * This returns pointer to the Push window.
 * @returns The push window.
 */
TrackGitWindow*
Push::GetWindow()
{
	if (fPushWindow == NULL)
		fPushWindow = new PushWindow(fRepo);
	return fPushWindow;
}


/**
 * This does git push on given repository. This also shows progress
 * in given progress window.
 * @param arg This contains repo path and progress window
 */
void*
DoPushThread(void* arg)
{
	struct push_params* p = (struct push_params*) arg;

	git_libgit2_init();
	
	int err;
	git_repository* repo = NULL;
	git_remote* remote = NULL;
	git_reference *head = NULL;
	git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
	git_push_options options;
	git_remote_autotag_option_t tags;
	git_strarray array;
	const char* ref_name;
	char refspec[200];

	err = git_repository_open(&repo, p->path);
	if (err < 0)
		goto ret;

	if (p->pushWindow)
		p->pushWindow->SetText("Getting remote...");
	err = git_remote_lookup(&remote, repo, "origin");
	if (err < 0)
		goto ret;

	callbacks.credentials = cred_acquire_cb;

	err = git_repository_head(&head, repo);
	if (err < 0)
		goto ret;

	ref_name = git_reference_name(head);
	sprintf(refspec, "%s:%s", ref_name, ref_name);

	err = git_push_init_options(&options, GIT_PUSH_OPTIONS_VERSION);
	if (err < 0)
		goto ret;
	memcpy(&options.callbacks, &callbacks, sizeof(git_remote_callbacks));

	if (p->pushWindow)
		p->pushWindow->SetText("Pushing changes...");
	
	array.count = 1;
	array.strings = (char**) malloc(sizeof(char*) * array.count);
	array.strings[0] = refspec;

	err = git_remote_push(remote, &array, &options);
	if (err == CANCEL_CREDENTIALS)
		goto clean;
	if (err < 0)
		goto ret;

	err = git_remote_update_tips(remote, NULL, 1, tags, NULL);

ret:
	if (err < 0) {
		const git_error* er = giterr_last();
		printf("Error %d : %s\n", er->klass, er->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", er->message);
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
				0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	}
clean:
	free(array.strings);
	git_reference_free(head);
	git_remote_disconnect(remote);
	git_remote_free(remote);
	git_repository_free(repo);
	git_libgit2_shutdown();
	if (p->pushWindow && p->pushWindow->LockLooper())
		p->pushWindow->Quit();
	free(arg);
}


/**
 * This spawns thread to perform push over given repo.
 * @param pushWindow The window to show progress.
 * @param path The repo path.
 */
pthread_t
Push::DoPush(PushWindow* pushWindow, const char* path)
{
	struct push_params *p = (struct push_params*)
			malloc(sizeof(struct push_params));
	p->path = path;
	p->pushWindow = pushWindow;
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, DoPushThread, p);
	return thread_id;
}
