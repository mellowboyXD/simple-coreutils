#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

#define ENOARG 1
#define ENOFILE 2
#define EISDIR 3
#define EMEMALLOC 4

enum bool { false, true };
enum options { DEFAULT, HELP, VERSION };

static uintmax_t char_count = 0;
static uintmax_t line_count = 0;
static uintmax_t letter_count = 0;
static uintmax_t word_count = 0;
static uintmax_t line_length = 0;
static uintmax_t longest_line = 0;
static uintmax_t longest_line_len = 0;

void help();
void version();
int isdir(const char *path);
const enum options *get_options(const char **argv, const int argc,
				char **fname);
void count(const char *fname, const enum options *opt);

int main(int argc, char **argv)
{
	if (argc < 2) {
		help();
		return ENOARG;
	}

	char *fname = argv[1];
	const enum options *opts =
		get_options((const char **)argv, argc, &fname);

	count(fname, opts);

	return 0;
}

int isdir(const char *path)
{
	struct stat pstat;
	stat(path, &pstat);
	return S_ISDIR(pstat.st_mode);
}

void help()
{
	printf("USAGE: swc [OPTIONS] [FILE]\n");
	printf("Need to implement proper help.\n");
}

void version()
{
	printf("swc (simple wc) 0.7\n");
	printf("Credits: mellowboyXD\n");
}

const enum options *get_options(const char **argv, const int argc, char **fname)
{
	const static enum options ret[] = { DEFAULT };
	return ret;
}

void count(const char *fname, const enum options *opt)
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

	size_t len = sizeof(opt) / sizeof(DEFAULT);
	for (size_t i = 0; i < len; i++) {
		if (opt[i] == DEFAULT) {
			printf("chars: %zu words: %zu lines: %zu ", char_count,
			       word_count, line_count);
			printf("longest-line: %zu longest-line-len: %zu %s\n",
			       longest_line, longest_line_len, fname);
			break;
		}
	}

	fclose(fptr);
};
