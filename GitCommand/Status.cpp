/**
 * @file Status.cpp
 * @brief Implemention file of Status command.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Status.h"
#include "../UI/StatusWindow.h"

#include <InterfaceKit.h>

#include <stdlib.h>
#include <strings.h>


/**
 * Status class Constructor.
 * @param repo The repo where the command is called.
 * @param dirPath The current directory where Status is selected.
 */
Status::Status(BString repo, BString dirPath)
	:
	GitCommand()
{
	fRepo = repo;
	fDirPath = dirPath;
	fStatusWindow = NULL;
}


/**
 * This returns pointer to the status window.
 * @returns The pointer to the Status Window.
 */
TrackGitWindow*
Status::GetWindow()
{
	if (fStatusWindow == NULL)
		fStatusWindow = new StatusWindow(fRepo);
	return fStatusWindow;
}


/**
 * Status command execution.
 * Shows an alert with status text.
 */
void
Status::Execute()
{
	BString* statusText = GetStatusText();

}


/**
 * Constructs Status text for current repo.
 * @param status The status info output of git_status_list_new.
 * @returns The Status text.
 */
BString*
Status::GetStatusTextUtil(git_status_list* status, StatusWindow* window)
{
	size_t i, maxi = git_status_list_entrycount(status);
	const git_status_entry *s;
	int header = 0, changesInIndex = 0;
	int changesInWorkDir = 0, rmInWorkDir = 0;
	const char *old_path, *new_path;
	BString* statusText = new BString();

	/** Print index changes. */
	for (i = 0; i < maxi; ++i) {
		char *istatus = NULL;

		s = git_status_byindex(status, i);

		if (s->status == GIT_STATUS_CURRENT)
			continue;

		if (s->status & GIT_STATUS_WT_DELETED)
			rmInWorkDir = 1;

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
			if (window) {
				window->AddItem(BString(), BString());
				window->AddItem(BString("Changes to be commited:"), BString());
			}
			header = 1;
		}

		old_path = s->head_to_index->old_file.path;
		new_path = s->head_to_index->new_file.path;

		BString text;
		if (old_path && new_path && strcmp(old_path, new_path)) {
			text.Append("\t%istatus %old -> %new\n");
			text.ReplaceFirst("%istatus", istatus);
			text.ReplaceFirst("%old", old_path);
			text.ReplaceFirst("%new", new_path);
			if (window)
				window->AddItem(text, BString(new_path));
		} else {
			text.Append("\t%istatus %file\n");
			text.ReplaceFirst("%istatus", istatus);
			text.ReplaceFirst("%file", old_path ? old_path : new_path);
			if (window)
				window->AddItem(text, BString(old_path ? old_path : new_path));
		}
		statusText->Append(text);
	}

	if (header) {
		changesInIndex = 1;
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
			if (window) {
				window->AddItem(BString(), BString());
				window->AddItem(BString("Changes not staged for commit:"),
							BString());
			}
		}

		old_path = s->index_to_workdir->old_file.path;
		new_path = s->index_to_workdir->new_file.path;

		BString text;
		if (old_path && new_path && strcmp(old_path, new_path)) {
			text.Append("\t%wstatus %old -> %new\n");
			text.ReplaceFirst("%wstatus", wstatus);
			text.ReplaceFirst("%old", old_path);
			text.ReplaceFirst("%new", new_path);
			if (window)
				window->AddItem(text, BString(new_path));
		} else {
			text.Append("\t%wstatus %file\n");
			text.ReplaceFirst("%wstatus", wstatus);
			text.ReplaceFirst("%file", old_path ? old_path : new_path);
			if (window)
				window->AddItem(text, BString(old_path ? old_path : new_path));
		}
		statusText->Append(text);
	}

	if (header) {
		changesInWorkDir = 1;
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
				if (window) {
					window->AddItem(BString(), BString());
					window->AddItem(BString("Untracked files:"), BString());
				}
			}

			BString text;
			text.Append("\tuntracked: %file\n");
			text.ReplaceFirst("%file" , s->index_to_workdir->old_file.path);
			statusText->Append(text);
			if (window)
				window->AddItem(text, BString(
						s->index_to_workdir->old_file.path));

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
				if (window) {
					window->AddItem(BString(), BString());
					window->AddItem(BString("Ignored files:"), BString());
				}
			}

			BString text;
			text.Append("\t%file\n");
			text.ReplaceFirst("%file" ,
					s->index_to_workdir->old_file.path);
			statusText->Append(text);
			if (window)
				window->AddItem(text, BString(
						s->index_to_workdir->old_file.path));
		}
	}

	if (!changesInIndex && changesInWorkDir) {
		statusText->Append("\nNo changes added to commit\n");
		if (window) {
			window->AddItem(BString(), BString());
			window->AddItem(BString("No changes added to commit."), BString());
		}
	}

	if (statusText->Length() == 0) {
		statusText->Append("No changes to current branch.\n");
		if (window)
			window->AddItem(BString("No changes to current branch."),
				   BString());
	}

	return statusText;
}


/**
 * Constructs Branch text for current repo.
 * @param repo The current repo.
 * @returns The Branch text.
 */
BString*
Status::GetBranchText(git_repository *repo, StatusWindow* window)
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
	if (window)
		window->AddItem(*branchText, BString());

	git_reference_free(head);
	
	return branchText;
}


/**
 * Constructs the entire Status Text along with current branch for given repo.
 * @param dirPath The current path to check status for.
 * @returns The entire status text to display.
 */
BString*
Status::GetStatusText()
{
	git_repository *repo = NULL;
	git_status_list *status;
	struct opts o = { GIT_STATUS_OPTIONS_INIT, fDirPath.String() };

	git_libgit2_init();

	o.statusopt.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
	o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
		GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
		GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
	o.statusopt.baseline = 0;

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

	BString* branchText = GetBranchText(repo, fStatusWindow);
	BString* statusText = GetStatusTextUtil(status, fStatusWindow);
	BString* text = new BString("Current directory: %dir\n");
	text->ReplaceFirst("%dir", fDirPath.String());
	text->Append(*branchText);
	text->Append(*statusText);

	git_libgit2_shutdown();
	
	return text;
}
