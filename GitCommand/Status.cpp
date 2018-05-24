/**
 * @file Status.cpp
 * @brief Implemention file of Status command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Status.h"

#include <InterfaceKit.h>
#include <SupportKit.h>

#include <stdlib.h>
#include <strings.h>


BString*
GetStatusTextUtil(git_status_list *status)
{
	size_t i, maxi = git_status_list_entrycount(status);
	const git_status_entry *s;
	int header = 0, changes_in_index = 0;
	int changed_in_workdir = 0, rm_in_workdir = 0;
	const char *old_path, *new_path;
	BString* statusText = new BString();

	/** Print index changes. */

	for (i = 0; i < maxi; ++i) {
		char *istatus = NULL;

		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_CURRENT)
			continue;

		if (s->status & GIT_STATUS_WT_DELETED)
			rm_in_workdir = 1;

		if (s->status & GIT_STATUS_INDEX_NEW)
			istatus = "new file: ";
		if (s->status & GIT_STATUS_INDEX_MODIFIED)
			istatus = "modified: ";
		if (s->status & GIT_STATUS_INDEX_DELETED)
			istatus = "deleted:  ";
		if (s->status & GIT_STATUS_INDEX_RENAMED)
			istatus = "renamed:  ";
		if (s->status & GIT_STATUS_INDEX_TYPECHANGE)
			istatus = "typechange:";

		if (istatus == NULL)
			continue;

		if (!header) {
			statusText->Append("\n");
			statusText->Append("Changes to be committed:\n");
			header = 1;
		}

		old_path = s->head_to_index->old_file.path;
		new_path = s->head_to_index->new_file.path;

		if (old_path && new_path && strcmp(old_path, new_path)) {
			statusText->Append("\t%istatus %old -> %new\n");
			statusText->ReplaceFirst("%istatus", istatus);
			statusText->ReplaceFirst("%old", old_path);
			statusText->ReplaceFirst("%new", new_path);
		} else {
			statusText->Append("\t%istatus %file\n");
			statusText->ReplaceFirst("%istatus", istatus);
			statusText->ReplaceFirst("%file", old_path ? old_path : new_path);
		}
	}

	if (header) {
		changes_in_index = 1;
	}
	header = 0;

	/** Print workdir changes to tracked files. */

	for (i = 0; i < maxi; ++i) {
		char *wstatus = NULL;

		s = git_status_byindex(status, i);

		/**
		 * With `GIT_STATUS_OPT_INCLUDE_UNMODIFIED` (not used in this example)
		 * `index_to_workdir` may not be `NULL` even if there are
		 * no differences, in which case it will be a `GIT_DELTA_UNMODIFIED`.
		 */
		if (s->status == GIT_STATUS_CURRENT || s->index_to_workdir == NULL)
			continue;

		/** Print out the output since we know the file has some changes */
		if (s->status & GIT_STATUS_WT_MODIFIED)
			wstatus = "modified: ";
		if (s->status & GIT_STATUS_WT_DELETED)
			wstatus = "deleted:  ";
		if (s->status & GIT_STATUS_WT_RENAMED)
			wstatus = "renamed:  ";
		if (s->status & GIT_STATUS_WT_TYPECHANGE)
			wstatus = "typechange:";

		if (wstatus == NULL)
			continue;

		if (!header) {
			statusText->Append("\n");
			statusText->Append("Changes not staged for commit:\n");
			header = 1;
		}

		old_path = s->index_to_workdir->old_file.path;
		new_path = s->index_to_workdir->new_file.path;

		if (old_path && new_path && strcmp(old_path, new_path)) {
			statusText->Append("\t%wstatus %old -> %new\n");
			statusText->ReplaceFirst("%wstatus", wstatus);
			statusText->ReplaceFirst("%old", old_path);
			statusText->ReplaceFirst("%new", new_path);
		} else {
			statusText->Append("\t%wstatus %file\n");
			statusText->ReplaceFirst("%wstatus", wstatus);
			statusText->ReplaceFirst("%file", old_path ? old_path : new_path);
		}
	}

	if (header) {
		changed_in_workdir = 1;
	}

	/** Print untracked files. */

	header = 0;

	for (i = 0; i < maxi; ++i) {
		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_WT_NEW) {

			if (!header) {
				statusText->Append("\n");
				statusText->Append("Untracked files:\n");
				header = 1;
			}

			statusText->Append("\t%file\n");
			statusText->ReplaceFirst("%file" , s->index_to_workdir->old_file.path);

		}
	}

	header = 0;

	/** Print ignored files. */

	for (i = 0; i < maxi; ++i) {
		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_IGNORED) {

			if (!header) {
				statusText->Append("\n");
				statusText->Append("Ignored files:\n");
				header = 1;
			}

			statusText->Append("\t%file\n");
			statusText->ReplaceFirst("%file" , s->index_to_workdir->old_file.path);
		}
	}

	if (!changes_in_index && changed_in_workdir)
		statusText->Append("\nNo changes added to commit\n");

	return statusText;
}



BString*
GetBranchText(git_repository *repo, int format)
{
	int error = 0;
	const char *branch = NULL;
	git_reference *head = NULL;

	error = git_repository_head(&head, repo);

	if (error == GIT_EUNBORNBRANCH || error == GIT_ENOTFOUND)
		branch = NULL;
	else if (!error) {
		branch = git_reference_shorthand(head);
	} else
		printf("Failed to get branch.\n");

	BString* branchText = new BString("Branch: ");
	branchText->Append((branch) ? branch : "No Branch info");
	branchText->Append("\n");

	git_reference_free(head);
	
	return branchText;
}


BString*
GetStatusText(char* dirPath)
{
	git_repository *repo = NULL;
	git_status_list *status;
	struct opts o = { GIT_STATUS_OPTIONS_INIT, dirPath };

	git_libgit2_init();

	o.statusopt.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
	o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
		GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
		GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;

	if (git_repository_open_ext(&repo, o.repodir, 0, NULL) != 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return NULL;
	}

	if (git_repository_is_bare(repo)) {
		BString buffer("Error : Cannot report status on bare repository.");
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return NULL;
	}

	if (git_status_list_new(&status, repo, &o.statusopt) != 0) {
		const git_error* err = giterr_last();
		printf("Error %d : %s\n", err->klass, err->message);

		BString buffer("Error : %s");
		buffer.ReplaceFirst("%s", err->message);
		BAlert* alert = new BAlert("", buffer.String(), "Cancel", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return NULL;
	}

	BString* branchText = GetBranchText(repo, o.format);
	BString* statusText = GetStatusTextUtil(status);
	BString* text = new BString(*branchText);
	text->Append(*statusText);
	
	return text;
}


/**
 * Status class Constructor.
 * @param dirPath The current directory where Status is selected.
 */
Status::Status(char* dirPath)
	:
	GitCommand()
{
	this->dirPath = dirPath;
}


/**
 * Status command execution. Opens a window to clone a repo.
 */
void
Status::Execute()
{
	BString* statusText = GetStatusText(dirPath);
	if (statusText) {
		BAlert* alert = new BAlert("", statusText->String(), "OK", 
			0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
	}
}
