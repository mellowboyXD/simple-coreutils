/* scp -- simple copy utility
 * This is an attempt to recreate the main part of the `cp` command in UNIX like
 * operating systems.
 *
 * POSIX Compliant and utilizing only direct systems calls like.
 *
 * Author: mellowboyxd */

#include <iostream>
#include <unistd.h>

#include "filedescriptor.hpp"

void print_usage();
bool copy_file(FileDescriptor &src, FileDescriptor &dst);
bool copy_permissions(FileDescriptor &src_fd, FileDescriptor &dst_fd);

int main(int argc, const char **argv)
{
	if (argc < 3) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	FileDescriptor src;
	FileDescriptor dst;
	if (!src.Open(argv[1], FileDescriptor::READ)) {
		std::cerr << src.getErrmsg() << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!dst.Open(argv[2], FileDescriptor::WRITE)) {
		std::cerr << dst.getErrmsg() << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!copy_file(src, dst)) {
		std::cerr << "An error occured during file copying."
			  << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!copy_permissions(src, dst)) {
		std::cerr << "Error copying permissions" << std::endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

void print_usage()
{
	std::cout << "Usage: scp <source> <destination>" << std::endl;
}

bool copy_file(FileDescriptor &src, FileDescriptor &dst)
{
	char buf[FileDescriptor::BUFSIZE];

	ssize_t bytes;
	while ((bytes = read(src.getFd(), buf, FileDescriptor::BUFSIZE)) > 0) {
		if (write(dst.getFd(), buf, bytes) != bytes) {
			std::cerr << "Error writing to file" << std::endl;
			return false;
		}
	}

	if (bytes < 0) {
		std::cerr << "Error reading file: " + src.getPathname()
			  << std::endl;
		return false;
	}

	return true;
}

bool copy_permissions(FileDescriptor &src, FileDescriptor &dst)
{
	std::string path = dst.getPathname();
	if (chmod(path.c_str(), src.getStat().st_mode) < 0) {
		std::cerr << "Error copy file mode" << std::endl;
		return false;
	}

	return true;
}
