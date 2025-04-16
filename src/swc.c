#include <stdint.h>
#include <sys/stat.h>
#include <stdio.h>

#define ENOARG 1
#define ENOFILE 2
#define EISDIR 3

enum bool { false, true };
enum options { DEFAULT, HELP, VERSION };

static uintmax_t char_count = 0;
static uintmax_t line_count = 0;
static uintmax_t letter_count = 0;
static uintmax_t word_count = 0;
static uintmax_t line_length = 0;
static uintmax_t longest_line = 0;
static uintmax_t longest_line_length = 0;

void help();
void version();
int isdir(const char *path);
enum options get_arguments(const char **argv, char **fname);

int main(int argc, char **argv)
{
	enum bool in_word = false;

	if (argc < 2) {
		help();
		return ENOARG;
	}

	char *fname = argv[1];

	FILE *fptr = fopen(fname, "r");
	if (fptr == NULL) {
		fprintf(stderr, "swc: %s: No such file or directory\n", fname);
		return ENOFILE;
	} else if (isdir(fname)) {
		fprintf(stderr, "swc: %s: Is a directory\n", fname);
		return EISDIR;
	}

	int c;
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
				if (line_length > longest_line_length) {
					longest_line_length = line_length;
					longest_line = line_count;
				}
				line_length = 0;
			}
		} else if (c == '\n') {
			++line_count;
			if (line_length > longest_line_length) {
				longest_line_length = line_length;
				longest_line = line_count;
			}
			line_length = 0;
		} else {
			in_word = true;
			++letter_count;
		}
	}

	printf("chars: %zu lines: %zu words: %zu longest-line: %zu longest-line-len: %zu %s\n",
	       char_count, line_count, word_count, longest_line,
	       longest_line_length, fname);
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
	printf("USAGE: swc [OPTIONS] [FILE]\n \
	Need to implement proper help.\n");
}

void version()
{
	printf("swc (simple wc) 0.6\n");
	printf("Credits: mellowboyXD\n");
}

enum options get_arguments(const char **argv, char **fname)
{
	return DEFAULT;
}
