/**
 * @file Commit.cpp
 * @brief Implementation of Commit command.
 *
 * @author Hrishikesh Hiraskar <hrishihiraskar@gmail.com>
 */

#include "Commit.h"
#include "../UI/CommitWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <git2.h>


Commit::Commit(BString repo)
{
	fRepo = repo;
	fCommitWindow = NULL;
}

void
Commit::Execute()
{
}

TrackGitWindow*
Commit::GetWindow()
{
	if (fCommitWindow == NULL)
		fCommitWindow = new CommitWindow(fRepo);
	return NULL;
}

int
Commit::DoCommit(BString repoPath, BString message)
{
	git_oid tree_id, commit_id, parent_id;
	git_repository* repo;
	git_signature* sign;
	git_index* index;
	git_tree* tree;
	git_commit* parent;
	int ret = 0;

	// Init repo
	ret = git_repository_open_ext(&repo, repoPath.String(), 0, NULL);
	if (ret < 0)
		return ret;

	git_config* cfg;
	git_config* cfg_snapshot;
	// git_repository_config_snapshot(&cfg, repo);
	git_config_open_ondisk(&cfg, "/boot/home/config/settings/git/config");
	git_config_snapshot(&cfg_snapshot, cfg);
	
	const char *user_name, *user_email;
	ret = git_config_get_string(&user_name, cfg_snapshot, "user.name");
	if (ret < 0)
		goto ret;

	ret = git_config_get_string(&user_email, cfg_snapshot, "user.email");
	if (ret < 0)
		goto ret;

	// Get default commiter
	ret = git_signature_now(&sign, user_name, user_email);
	if (ret < 0)
		goto ret;

	// Init index
	ret = git_repository_index(&index, repo);
	if (ret < 0)
		goto ret;
	
	// Init tree_id
	ret = git_index_write_tree(&tree_id, index);
	if (ret < 0)
		goto ret;

	git_index_free(index);

	// Init tree
	ret = git_tree_lookup(&tree, repo, &tree_id);
	if (ret < 0)
		goto ret;

	ret = git_reference_name_to_id(&parent_id, repo, "HEAD");
	if (ret < 0) {
		// No parent commit found. This might be initial commit.
		ret = git_commit_create_v(&commit_id, repo, "HEAD", sign, sign, NULL,
				message.String(), tree, 0);
		goto ret;
	}


	ret = git_commit_lookup(&parent, repo, &parent_id);
	if (ret < 0)
		goto ret;

	// Create commit
	ret = git_commit_create_v(&commit_id, repo, "HEAD", sign, sign, NULL,
			message.String(), tree, 1, parent);

ret:
	git_tree_free(tree);
	git_signature_free(sign);
	git_repository_free(repo);

	git_libgit2_shutdown();

	return ret;
}
