/*
 * Copyright 2018, Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "Log.h"

#include <stdlib.h>
#include <strings.h>

#include <InterfaceKit.h>

#include "../UI/LogWindow.h"


/**
 * Log class Constructor.
 * @param repo The repo where the command is called.
 * @param dirPath The current directory where Log is selected.
 */
Log::Log(BString repo)
	:
	GitCommand()
{
	fRepo = repo;
	fLogWindow = NULL;
}


/**
 * This returns pointer to the log window.
 * @returns The pointer to the Log Window.
 */
TrackGitWindow*
Log::GetWindow()
{
	if (fLogWindow == NULL)
		fLogWindow = new LogWindow(fRepo);
	return fLogWindow;
}


/**
 * Log command execution.
 * Opens an alert with log text.
 */
void
Log::Execute()
{
	if (!fLogWindow) {
		fLogWindow->Quit();
		return;
	}
	AppendLogText();
}


/**
 * Checks if libgit2 api was successfully executed.
 * Prints error if not.
 * @param error The error code.
 * @param message The error message.
 * @param extra Any extra message.
 */
void check_lg2(int error, const char* message, const char* extra)
{
	const char *lg_msg = "";
	if (!error)
		return;

	const git_error* err = giterr_last();
	if (err)
		lg_msg = err->message;

	BString buffer("Error : %s. %s");
	buffer.ReplaceFirst("%s", message);
	buffer.ReplaceFirst("%s", lg_msg);
	BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
}


/**
 * Helper to format a git_time value like Git.
 */
static void print_time(const git_time *intime, const char *prefix,
		BString &text)
{
	char sign, out[32];
	struct tm *intm;
	int offset, hours, minutes;
	time_t t;

	offset = intime->offset;
	if (offset < 0) {
		sign = '-';
		offset = -offset;
	} else {
		sign = '+';
	}

	hours   = offset / 60;
	minutes = offset % 60;

	t = (time_t)intime->time + (intime->offset * 60);

	intm = gmtime(&t);
	strftime(out, sizeof(out), "%a %b %e %T %Y", intm);

	text << prefix << out << " "<< sign << hours << minutes << "\n";
}


/**
 * Helper to print a commit object.
 */
static void print_commit(git_commit *commit, struct log_options *opts,
		BString &text)
{
	char buf[GIT_OID_HEXSZ + 1];
	int i, count;
	const git_signature *sig;
	const char *scan, *eol;

	git_oid_tostr(buf, sizeof(buf), git_commit_id(commit));
	text << "commit " << buf << "\n";

	if ((count = (int)git_commit_parentcount(commit)) > 1) {
		text << "Merge: ";
		for (i = 0; i < count; ++i) {
			git_oid_tostr(buf, 8, git_commit_parent_id(commit, i));
			text << buf;
		}
		text << "\n";
	}

	if ((sig = git_commit_author(commit)) != NULL) {
		text << "Author: " << sig->name << " <" << sig->email << ">\n";
		print_time(&sig->when, "Date:   ", text);
	}

	text << "\n" << git_commit_message(commit) << "\n\n";
}


/**
 * Push object (for hide or show) onto revwalker.
 */
static void push_rev(struct log_state *s, git_object *obj, int hide)
{
	hide = s->hide ^ hide;

	/** Create revwalker on demand if it doesn't already exist. */
	if (!s->walker) {
		check_lg2(git_revwalk_new(&s->walker, s->repo),
			"Could not create revision walker", NULL);
		git_revwalk_sorting(s->walker, s->sorting);
	}

	if (!obj)
		check_lg2(git_revwalk_push_head(s->walker),
			"Could not find repository HEAD", NULL);
	else if (hide)
		check_lg2(git_revwalk_hide(s->walker, git_object_id(obj)),
			"Reference does not refer to a commit", NULL);
	else
		check_lg2(git_revwalk_push(s->walker, git_object_id(obj)),
			"Reference does not refer to a commit", NULL);

	git_object_free(obj);
}

/**
 * Parse revision string and add revs to walker.
 */
static int add_revision(struct log_state *s, const char *revstr)
{
	git_revspec revs;
	int hide = 0;

	/** Open repo on demand if it isn't already open. */
	if (!s->repo) {
		if (!s->repodir) s->repodir = ".";
		check_lg2(git_repository_open_ext(&s->repo, s->repodir, 0, NULL),
			"Could not open repository", s->repodir);
	}

	if (!revstr) {
		push_rev(s, NULL, hide);
		return 0;
	}

	if (*revstr == '^') {
		revs.flags = GIT_REVPARSE_SINGLE;
		hide = !hide;

		if (git_revparse_single(&revs.from, s->repo, revstr + 1) < 0)
			return -1;
	} else if (git_revparse(&revs, s->repo, revstr) < 0)
		return -1;

	if ((revs.flags & GIT_REVPARSE_SINGLE) != 0)
		push_rev(s, revs.from, hide);
	else {
		push_rev(s, revs.to, hide);

		if ((revs.flags & GIT_REVPARSE_MERGE_BASE) != 0) {
			git_oid base;
			check_lg2(git_merge_base(&base, s->repo,
				git_object_id(revs.from), git_object_id(revs.to)),
				"Could not find merge base", revstr);
			check_lg2(
				git_object_lookup(&revs.to, s->repo, &base, GIT_OBJ_COMMIT),
				"Could not find merge base commit", NULL);

			push_rev(s, revs.to, hide);
		}

		push_rev(s, revs.from, !hide);
	}

	return 0;
}


/**
 * Appends the Log Text for repo to the Log Window
 */
void
Log::AppendLogText()
{
	git_repository *repo = NULL;
	git_pathspec *ps = NULL;

	git_libgit2_init();

	struct log_options opt;
	memset(&opt, 0, sizeof(opt));
	opt.max_parents = -1;
	opt.limit = -1;

	struct log_state state;
	memset(&state, 0, sizeof(state));
	state.sorting = GIT_SORT_TIME;
	state.repodir = fRepo.String();

	if (!state.revisions)
		add_revision(&state, NULL);

	/** Use the revwalker to traverse the history. */

	BString text;
	git_oid oid;
	git_commit *commit = NULL;

	for (int i = -1; !git_revwalk_next(&oid, state.walker); git_commit_free(commit)) {
		check_lg2(git_commit_lookup(&commit, state.repo, &oid),
			"Failed to look up commit", NULL);

		// We only append every ~100 commits because the delay is unnoticable,
		// and infrequent appends are a bit less heavy.
		print_commit(commit, &opt, text);
		if (i == -1 || i == 100) {
			fLogWindow->AppendText(text);
			text.SetTo("");
			i = -1;
		}
		i++;
	}
	fLogWindow->AppendText(text);

	git_pathspec_free(ps);
	git_repository_free(repo);
	git_libgit2_shutdown();
}
