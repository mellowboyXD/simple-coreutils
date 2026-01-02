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
#include "copy.hpp"

void print_usage()
{
	std::cout << "Usage: scp <source> <destination>" << std::endl;
}

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

	if (!copy::copy_file(src, dst)) {
		std::cerr << "Error copying file contents" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!copy::copy_permissions(src, dst)) {
		std::cerr << "Error copying permissions" << std::endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
