#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "util.h"

char buf[1024];
int last = 0;
char info[1024];
enum { STOPPED , PAUSED, PLAYING};
int cmusst;

/* run_command but with newline */
const char *
run_command_nl(const char *cmd)
{
	char *p;
	FILE *fp;
	char *start = &buf[0];
	if (!(fp = popen(cmd, "r"))) {
		warn("popen '%s':", cmd);
		return NULL;
	}
	while ((*p = fgetc(fp)) != EOF) {
		*start = *p;
		start++;
	}
	if (pclose(fp) < 0) {
		warn("pclose '%s':", cmd);
		return NULL;
	}
	if (!p) {
		return NULL;
	}
	if ((p = strrchr(buf, '\n'))) {
		p[0] = '\0';
	}

	return buf[0] ? buf : NULL;
}

void config_status(const char* result) {
	if (strstr(result, "status stopped")) {
		cmusst = STOPPED;
	} else if (strstr(result, "status paused")) {
		cmusst = PAUSED;
	} else if (strstr(result, "status playing")) {
		cmusst = PLAYING;
	}
}

void add_info(const char *result, const char *strstart, char *current) {
	char *infostart = strstr(result, strstart);
	int len = strlen(strstart);
	char *c;
	infostart += len;
	for (;;) {
		c = infostart;
		if (*c == '\n' || *c == EOF) {
			*current = '\0';
			break;
		}
		*current = *c;
		current++;
		infostart++;
	}
}

void fill_info(const char *result) {
	char *begining = &info[0];
	info[0] = '\0';
	add_info(result, "tag artist ", begining);
	add_info(result, "tag title ", begining);
	printf("%s", info);
}

const char *
query_cmus() {
	const char *result = run_command_nl("cmus-remote -Q");
	if (result == NULL) {
		warn("run_command '%s':", "-");
		return NULL;
	}
	config_status(result);
	fill_info(result);
	return NULL;
}

int main (int argc, char *argv[]) {
	query_cmus();
	return 0;
}
