#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>

static const int range = 2 * 60 * 60;

static char tree[64], parent[64];
static char author[128], amail[128], atz[8];
static char committer[128], cmail[128], ctz[8];
static char msg[2048], tmp[1024];
static unsigned char sha1[20];
static size_t msg_len;
static unsigned long long atime, ctime;

static int setmsg(unsigned long long atime, unsigned long long ctime)
{
	int len, len2;

	if (parent[0] != '\0') {
		len = sprintf(tmp, "tree %s\nparent %s\n"
		                   "author %s %s %llu %s\n"
		                   "committer %s %s %llu %s\n",
		              tree, parent,
		              author, amail, atime, atz,
		              committer, cmail, ctime, ctz) + 1;
	} else {
		len = sprintf(tmp, "tree %s\n"
		                   "author %s %s %llu %s\n"
		                   "committer %s %s %llu %s\n",
		              tree,
		              author, amail, atime, atz,
		              committer, cmail, ctime, ctz) + 1;
	}
	strcpy(msg + 1024 - len, tmp);
	msg[1024 - 1] = '\n';

	len2 = sprintf(tmp, "commit %zu", msg_len + len) + 1;
	strcpy(msg + 1024 - len - len2, tmp);
	msg[1024 - len - 1] = '\0';

	return len + len2;
}

int main(void)
{
	FILE *fp;
	int len, i, j;

	fp = popen("git cat-file commit HEAD", "r");
	if (fp == NULL) {
		return 1;
	}
	fscanf(fp, "tree %s%s", tree, tmp);
	if (strcmp(tmp, "parent") == 0) {
		fscanf(fp, "%s\nauthor %s %s %llu %s\n",
		       parent, author, amail, &atime, atz);
	} else {
		fscanf(fp, "%s %s %llu %s\n",
		       author, amail, &atime, atz);
	}
	fscanf(fp, "committer %s %s %llu %s\n\n",
	       committer, cmail, &ctime, ctz);
	msg_len = fread(msg + 1024, 1, sizeof(msg) - 1024, fp);
	msg[msg_len] = '\0';
	pclose(fp);

	for (i = 0; i < range; i++) {
		for (j = i; j < range; j++) {
			len = setmsg(atime - j, ctime - i);
			SHA1(msg + 1024 - len, msg_len + len, sha1);
			if (sha1[0] == 0 && sha1[1] == 0 && sha1[2] == 0) {
				sprintf(tmp, "GIT_COMMITTER_DATE='%llu %s' "
				             "git commit --amend --no-edit --date '%llu %s'",
				        ctime - i, ctz, atime - j, atz);
				return execl("/bin/sh", "sh", "-c", tmp, NULL);
			}
		}
	}

	return 1;
}
