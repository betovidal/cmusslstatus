#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "util.h"
#include <unistd.h>

char buf[1024];
int last = 0;
char info[1024];
char timeinfo[7]; /* No audio files over 9 hours! No sir! */
char secinfo[20]; /* Because lhgjklhasljkda */
enum { STOPPED , PAUSED, PLAYING};
int cmusst;

/* run_command but with newline */
const char *
run_command_nl(const char *cmd)
{
	char *p;
	char rchar;
	FILE *fp;
	char *start = &buf[0];
	if (!(fp = popen(cmd, "r"))) {
		warn("popen '%s':", cmd);
		return NULL;
	}
	while ((rchar = fgetc(fp)) != EOF) {
		*start = rchar;
		start++;
	}
	if (pclose(fp) < 0) {
		warn("pclose '%s':", cmd);
		return NULL;
	}
	if (!rchar) {
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

void append_str(char **current, const char *str) {
	for (;;) {
		/* printf("(%c)", str[i]); */
		*((*current)++) = *(str++);
		if (*str == '\0') {
			break;
		}
	}
}

void add_info(const char *result, const char *strstart, char **current) {
	char *infostart = strstr(result, strstart);
	int len = strlen(strstart);
	char *c;
	if (!infostart) {
		return;
	}
	infostart += len;
	c = infostart;
	for (;;) {
		if (*c == '\n' || *c == EOF) {
			**current = '\0';
			break;
		}
		*((*current)++) = *(c++);
	}
}

void gen_time_format() {
	int hours = 0, minutes = 0, seconds = atoi(secinfo);
	int ressec, resmin;
	ressec = seconds % 60;
	minutes = (seconds - ressec) / 60;
	resmin = minutes % 60;
	hours = (minutes - resmin) / 60;
	printf("\n%d hours, %d minutes and %d seconds.\n", hours, resmin, ressec);
}


void fill_info(const char *result) {
	char *infostart = &info[0];
	char *secstart = &secinfo[0];
	info[0] = '\0';
	secinfo[0] = '\0';
	/* info */
	add_info(result, "\ntag artist ", &infostart);
	append_str(&infostart, " - ");
	add_info(result, "\ntag title ", &infostart);
	printf("%s", info);
	/* secinfo */
	add_info(result, "\nposition ", &secstart);
	gen_time_format();
}

void fill_time(const char *result) {
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
	fill_time(result);
	return NULL;
}

int main (int argc, char *argv[]) {
	while (1) {
		query_cmus();
		sleep(1);
	}
	return 0;
}
