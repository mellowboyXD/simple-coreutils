#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

#define VERSION "v0.8"

#define ENOARG 1
#define ENOFILE 2
#define EISDIR 3
#define EMEMALLOC 4

#define MAXOPTIONS 2
#define MAXCMDLEN 1024

void help();
void version();
void exit_error();
void invalid_option(char *opt);
int isdir(const char *path);

enum bool { false, true };

struct options {
	char *name;
	enum bool has_arg;
	int val;
};

static uintmax_t char_count = 0;
static uintmax_t line_count = 0;
static uintmax_t letter_count = 0;
static uintmax_t word_count = 0;
static uintmax_t line_length = 0;
static uintmax_t longest_line = 0;
static uintmax_t longest_line_len = 0;

static struct options long_options[] = { { "help", false, 'h' },
					 { "version", false, 'v' },
					 { 0, 0, 0 } };

void remove_flag_delim(char **arg, int start, int stop, char *out)
{
	int n = 0;
	for (int i = start; i < stop; i++) {
		out[n] = (*arg)[i];
		n++;
	}
}

int getopt(int argc, char **argv, struct options long_options[], char *opt_name)
{
	for (int i = 1; i < argc; i++) {
		printf("argv[i]: %s\n", argv[i]);
		char out[MAXCMDLEN] = "";
		char flag_delim = '-';
		if (strlen(argv[i]) <= 1 || strcmp(argv[i], "--") == 0)
			return -1;

		if (argv[i][1] == flag_delim)
			remove_flag_delim(&argv[i], 2, strlen(argv[i]), out);
		else if (argv[i][0] == flag_delim)
			remove_flag_delim(&argv[i], 1, 2, out);

		printf("out len: %zu\n", strlen(out));

		if (strlen(out) > 0) {
			strcpy(opt_name, out);
			for (int j = 0; j < MAXOPTIONS; j++) {
				if (strcmp(out, long_options[j].name) == 0) {
					return long_options[j].val;
				}
			}
			return -2;
		} else {
			strcpy(opt_name, argv[i]);
			return -2;
		}
	}
	return -1;
}

int getfilename(int argc, char **argv, char *out)
{
	if (argc < 2)
		return -1;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			continue;
		strcpy(out, argv[i]);
		return 0;
	}

	return 0;
}

void count(const char *fname)
{
	enum bool in_word = false;

	FILE *fptr = fopen(fname, "r");
	if (fptr == NULL) {
		fprintf(stderr, "swc: %s: No such file or directory\n", fname);
		exit(ENOFILE);
	} else if (isdir(fname)) {
		fprintf(stderr, "swc: %s: Is a directory\n", fname);
		exit(EISDIR);
	}

	int c = 0;
	while ((c = fgetc(fptr)) != EOF) {
		++char_count;
		line_length = line_length + 1;
		if (in_word) {
			++letter_count;
			if (c == ' ' || c == '\t') {
				in_word = false;
				++word_count;
			} else if (c == '\n') {
				in_word = false;
				++word_count;
				++line_count;
				if (line_length > longest_line_len) {
					longest_line_len = line_length;
					longest_line = line_count;
				}
				line_length = 0;
			}
		} else if (c == '\n') {
			++line_count;
			if (line_length > longest_line_len) {
				longest_line_len = line_length;
				longest_line = line_count;
			}
			line_length = 0;
		} else {
			in_word = true;
			++letter_count;
		}
	}

	fclose(fptr);
};

int main(int argc, char **argv)
{
	char opt_name[MAXCMDLEN] = "";
	char fname[MAXCMDLEN] = "";

	if (getfilename(argc, argv, fname) != 0)
		exit_error();

	int c;
	while ((c = getopt(argc, argv, long_options, opt_name)) != -1) {
		switch (c) {
		case 'h':
			help();
			return 0;
		case 'v':
			version();
			return 0;
		default:
			invalid_option(opt_name);
			exit(ENOARG);
		}
	}

	if (c == -1) {
		exit_error();
	}

	count(fname);
	return 0;
}

int isdir(const char *path)
{
	struct stat pstat;
	stat(path, &pstat);
	return S_ISDIR(pstat.st_mode);
}

void exit_error()
{
	fprintf(stderr, "swc: No arguments provided\n");
	fprintf(stderr, "Try 'swc --help' for more information\n");
	exit(ENOARG);
}

void help()
{
	printf("USAGE: swc [OPTIONS] [FILE]\n");
	printf("Need to implement proper help.\n");
}

void version()
{
	printf("swc (simple wc) %s\n", VERSION);
	printf("Credits: mellowboyXD\n");
}

void invalid_option(char *opt)
{
	fprintf(stderr, "swc: invalid option '%s'\n", opt);
	fprintf(stderr, "Try 'swc --help' for more information\n");
}
