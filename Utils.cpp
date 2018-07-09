/**
 * @file Utils.cpp
 * @brief Implementation file of Utils.
 *		  This file contains common functions and enums used by app.
 * 
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Utils.h"
#include "UI/ConflictsWindow.h"
#include "UI/CredentialsWindow.h"

#include <StorageKit.h>

#include <git2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Get current directory.
 * @param msg The BMessage containing ref to current directory.
 * @return The current directory.
 */
BString
extract_current_directory(const BMessage* msg)
{	
	// Get current directory path.
	entry_ref dirRef;
	if (msg->FindRef("dir_ref", &dirRef) != B_OK) {
		printf("No dir_ref found!\n");
		return NULL;
	}
	BEntry entry;
	entry.SetTo(&dirRef);
	BPath path;
	entry.GetPath(&path);
	return BString(path.Path());
}


/**
 * Get selected files.
 * @param msg The BMessage containing refs to selected files.
 * @param selected The vector of selected file paths.
 */
void
extract_selected_paths(const BMessage* msg, vector<char*>& selected)
{
	// Get current directory
	BString dir = extract_current_directory(msg);
	// Get all the selected refs
	entry_ref fileRef;
	for (int refs=0;
			 msg->FindRef("refs", refs, &fileRef) == B_NO_ERROR;
			 refs++) {
		BEntry entry;
		entry.SetTo(&fileRef);
		BPath path;
		entry.GetPath(&path);
		BString relativePath(path.Path());
		// Generate relative path from absolute path
		relativePath.ReplaceFirst(dir, "");
		relativePath.ReplaceFirst("/", "");
		// To convert const char* to char*
		char* p = (char*) malloc(relativePath.Length());
		strcpy(p, relativePath.String());
		selected.push_back(p);
	}
}


/**
 * Get the root of repo of current directory.
 * @param dirPath The current directory.
 * @returns The repo root if found else current directory.
 */
BString
get_root_of_repo(BString dirPath)
{
	git_libgit2_init();
	git_buf buf = GIT_BUF_INIT_CONST(NULL, 0);

	// Check if current directory is in git repo.
	int ret = git_repository_discover(&buf, dirPath.String(), 0, NULL);
	git_libgit2_shutdown();

	// Return root of repo if current directory is in git repo
	// else return current directory.
	return (ret == 0) ? BString(buf.ptr) : dirPath;
}


/**
 * This performs fastforward on given repo, branch id.
 * @param repo The git repository.
 * @param target_oid The target branch id.
 * @param is_unborn True if HEAD isn't born yet.
 * @returns 0 or error code.
 */
int perform_fastforward(git_repository *repo, const git_oid *target_oid,
		int is_unborn)
{
	git_checkout_options ff_checkout_options = GIT_CHECKOUT_OPTIONS_INIT;
	git_reference *target_ref;
	git_reference *new_target_ref;
	git_object *target = NULL;
	int err = 0;

	if (is_unborn) {
		const char *symbolic_ref;
		git_reference *head_ref;

		/* HEAD reference is unborn, 
		 * lookup manually so we don't try to resolve it
		 */
		err = git_reference_lookup(&head_ref, repo, "HEAD");
		if (err != 0) {
				fprintf(stderr, "failed to lookup HEAD ref\n");
				return -1;
		}

		/* Grab the reference HEAD should be pointing to */
		symbolic_ref = git_reference_symbolic_target(head_ref);

		/* Create our master reference on the target OID */
		err = git_reference_create(&target_ref, repo, symbolic_ref,
				target_oid, 0, NULL);
		if (err != 0) {
				fprintf(stderr, "failed to create master reference\n");
				return -1;
		}

		git_reference_free(head_ref);
	} else {
		/* HEAD exists, just lookup and resolve */
		err = git_repository_head(&target_ref, repo);
		if (err != 0) {
				fprintf(stderr, "failed to get HEAD reference\n");
				return -1;
		}
	}

	/* Lookup the target object */
	err = git_object_lookup(&target, repo, target_oid, GIT_OBJ_COMMIT);
	if (err != 0) {
		fprintf(stderr, "failed to lookup OID %s\n",
				git_oid_tostr_s(target_oid));
		return -1;
	}

	/* Checkout the result so the workdir is in the expected state */
	ff_checkout_options.checkout_strategy = GIT_CHECKOUT_SAFE;
	err = git_checkout_tree(repo, target, &ff_checkout_options);
	if (err != 0) {
			fprintf(stderr, "failed to checkout HEAD reference\n");
			return -1;
	}

	/* Move the target reference to the target OID */
	err = git_reference_set_target(&new_target_ref, target_ref, target_oid,
			NULL);
	if (err != 0) {
		fprintf(stderr, "failed to move HEAD reference\n");
		return -1;
	}

	git_reference_free(target_ref);
	git_reference_free(new_target_ref);
	git_object_free(target);

	return 0;
}


/**
 * Creates a commit on given repo, index and message.
 * @param repo The git repository.
 * @param index The git index.
 * @param message The commit message.
 * @returns 0 or error code.
 */
int
create_commit(git_repository* repo, git_index* index, const char* message)
{
	git_tree* tree;
	git_commit* parent;
	git_oid tree_id, commit_id, parent_id;
	git_signature* sign;
	git_config* cfg;
	git_config* cfg_snapshot;
	int ret;

	git_config_open_ondisk(&cfg, "/boot/home/config/settings/git/config");
	git_config_snapshot(&cfg_snapshot, cfg);
	
	const char *user_name, *user_email;
	ret = git_config_get_string(&user_name, cfg_snapshot, "user.name");
	if (ret < 0)
		return ret;

	ret = git_config_get_string(&user_email, cfg_snapshot, "user.email");
	if (ret < 0)
		return ret;

	// Get default commiter
	ret = git_signature_now(&sign, user_name, user_email);
	if (ret < 0)
		return ret;
	
	// Init tree_id
	ret = git_index_write_tree(&tree_id, index);
	if (ret < 0)
		return ret;

	git_index_free(index);

	// Init tree
	ret = git_tree_lookup(&tree, repo, &tree_id);
	if (ret < 0)
		return ret;

	ret = git_reference_name_to_id(&parent_id, repo, "HEAD");
	if (ret < 0) {
		// No parent commit found. This might be initial commit.
		ret = git_commit_create_v(&commit_id, repo, "HEAD", sign, sign, NULL,
				message, tree, 0);
		return ret;
	}

	ret = git_commit_lookup(&parent, repo, &parent_id);
	if (ret < 0)
		return ret;

	// Create commit
	ret = git_commit_create_v(&commit_id, repo, "HEAD", sign, sign, NULL,
			message, tree, 1, parent);
	return ret;
}


/**
 * Shows conflicts in current repo index in a window.
 * @param index The git index with conflicts.
 */
void output_conflicts(git_index *index)
{
	ConflictsWindow* conflictsWindow = new ConflictsWindow();
	git_index_conflict_iterator *conflicts;
	const git_index_entry *ancestor;
	const git_index_entry *our;
	const git_index_entry *their;
	int err = 0;
	BString conflictsString;

	err = git_index_conflict_iterator_new(&conflicts, index);
	if (err < 0) {
		conflictsString << "Failed to create conflict iterator!\n";
		goto ret;
	}

	while ((err = git_index_conflict_next(&ancestor, &our, &their, conflicts))
			== 0) {
		conflictsString << "Conflict in file: "
			<< " a: " << (ancestor ? ancestor->path : "NULL")
			<< " o: " << (our->path ? our->path : "NULL")
			<< " t: " << (their->path ? their->path : "NULL")
			<< "\n";
	}

	if (err != GIT_ITEROVER)
		conflictsString << "Error iterating conflicts\n";

	git_index_conflict_iterator_free(conflicts);

	conflictsString << "\n"
	                << "  a : Ancestor path\n"
	                << "  o : Our path\n"
	                << "  t : Their path\n";
	conflictsString << "Resolve the conflicts and add files to mark them as "
		<< "resolved.\nCommit changes after resolving all files.\n";
ret:
	printf("%s\n", conflictsString.String());
	conflictsWindow->SetText(conflictsString);

	thread_id thread = conflictsWindow->Thread();
	// wait for widow thread to return
	status_t win_status = B_OK;
	wait_for_thread(thread, &win_status);
}


/**
 * This functions gets username and password from the user in case
 * credentials are required.
 */
int cred_acquire_cb(git_cred** out, const char* url,
		const char* username_from_url, unsigned int allowed_types,
		void* payload)
{
	char username[100] = "",
		 password[100] = "";
	CredentialsWindow* window = new CredentialsWindow(username, password);

	thread_id thread = window->Thread();
	// wait for widow thread to return
	status_t win_status = B_OK;
	wait_for_thread(thread, &win_status);

	return git_cred_userpass_plaintext_new(out, username, password);
}
