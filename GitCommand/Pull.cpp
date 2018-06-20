/**
 * @file Pull.cpp
 * @brief Implementaion file of Pull command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Pull.h"
#include "../Utils.h"

#include <InterfaceKit.h>

#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Pull command constructor.
 * @param dirPath The directory where Init is selected.
 */
Pull::Pull(BString repo)
	:
	GitCommand()
{
	fRepo = repo;
	fPullWindow = NULL;
}


void
Pull::Execute()
{
}

TrackGitWindow*
Pull::GetWindow()
{
	if (fPullWindow == NULL)
		fPullWindow = new PullWindow(fRepo);
	return fPullWindow;
}

int fetchhead_ref_cb(const char* name, const char* url, const git_oid* oid, 
		unsigned int is_merge, void* payload_v) {
	struct fetch_payload* payload = (struct fetch_payload*) payload_v;
	if (is_merge) {
		strcpy(payload->branch, name);
		memcpy(&payload->branch_oid, oid, sizeof(git_oid));
	}
}

void*
DoPullThread(void* arg)
{
	struct pull_params* p = (struct pull_params*) arg;

	git_libgit2_init();

	git_repository* repo = NULL;
	git_remote *remote;
	int err;

	if (p->pullWindow)
		p->pullWindow->SetText("Pulling repository...");
	err = git_repository_open(&repo, p->path);
	if (err < 0)
		goto ret;

	printf("Getting remote\n");
	err = git_remote_lookup(&remote, repo, "origin");
	if (err < 0) {
		BString buffer("Error : No remote found.");
		BAlert *alert = new BAlert("", buffer.String(), "Cancel", 
				0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		goto clean;
	}

	printf("Fetching...\n");
	if (p->pullWindow)
		p->pullWindow->SetText("Fetching changes...");
	err = git_remote_fetch(remote, NULL, NULL, NULL);
	if (err < 0)
		goto ret;

	err = git_remote_fetch(remote, NULL, NULL, NULL);
	if (err < 0)
		goto ret;

	struct fetch_payload payload;
	git_repository_fetchhead_foreach(repo, fetchhead_ref_cb, &payload);

	git_annotated_commit* heads[1];
	err = git_annotated_commit_lookup(&heads[0], repo, &payload.branch_oid);
	if (err < 0)
		goto ret;

	if (p->pullWindow)
		p->pullWindow->SetText("Checking for merge...");
	git_merge_analysis_t merge_analysis_t;
   	git_merge_preference_t merge_preference_t;
	err = git_merge_analysis(&merge_analysis_t, &merge_preference_t,
							 repo, (const git_annotated_commit**)&heads[0], 1);

	if (merge_analysis_t & GIT_MERGE_ANALYSIS_UP_TO_DATE) {
		BString buffer("Repository up to date.");
		BAlert *alert = new BAlert("", buffer.String(), "OK", 
				0, 0, B_WIDTH_AS_USUAL);
		alert->Go();
		goto ret;
	} else if (merge_analysis_t & GIT_MERGE_ANALYSIS_FASTFORWARD) {
		if (p->pullWindow)
			p->pullWindow->SetText("Performing fast-forward...");
		err = perform_fastforward(repo, &payload.branch_oid,
					(merge_analysis_t & GIT_MERGE_ANALYSIS_UNBORN));
		goto ret;
	} else if (merge_analysis_t & GIT_MERGE_ANALYSIS_NORMAL) {
		if (p->pullWindow)
			p->pullWindow->SetText("Performing merge...");
		git_merge_options merge_opts = GIT_MERGE_OPTIONS_INIT;
		git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

		merge_opts.file_flags = GIT_MERGE_FILE_STYLE_DIFF3;

		checkout_opts.checkout_strategy = GIT_CHECKOUT_FORCE | 
											GIT_CHECKOUT_ALLOW_CONFLICTS;

		err = git_merge(repo,
						(const git_annotated_commit **)&heads[0], 1,
						&merge_opts, &checkout_opts);
		if (err < 0)
			goto ret;
	}

	/* If we get here, we actually performed the merge above */
	git_index* index;
	err = git_repository_index(&index, repo);
	if (err < 0)
		goto ret;

	if (git_index_has_conflicts(index)) {
		/* Handle conflicts */
		p->pullWindow->Hide();
		output_conflicts(index);
	} else {
		if (p->pullWindow)
			p->pullWindow->SetText("Applying commit...");
		create_commit(repo, index, "Merge");
		printf("Merge made\n");
	}

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
	git_repository_free(repo);
	git_libgit2_shutdown();
	if (p->pullWindow && p->pullWindow->LockLooper())
		p->pullWindow->Quit();
	free(arg);
}

pthread_t
Pull::DoPull(PullWindow* pullWindow, const char* path)
{
	struct pull_params *p = (struct pull_params*)
			malloc(sizeof(struct pull_params));
	p->path = path;
	p->pullWindow = pullWindow;
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, DoPullThread, p);
	return thread_id;
}
