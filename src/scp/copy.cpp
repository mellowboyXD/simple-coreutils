#include "filedescriptor.hpp"
#include <iostream>
#include "copy.hpp"

bool copy::copy_file(FileDescriptor &src, FileDescriptor &dst)
{
	char buf[FileDescriptor::BUFSIZE];

	ssize_t bytes;
	while ((bytes = read(src.getFd(), buf, FileDescriptor::BUFSIZE)) > 0) {
		if (write(dst.getFd(), buf, bytes) != bytes) {
			std::cerr << "Error writing to file: ";
			return false;
		}
	}

	if (bytes < 0) {
		std::cerr << "Error reading file: " + src.getPathname() << ": ";
		return false;
	}

	return true;
}

bool copy::copy_permissions(FileDescriptor &src, FileDescriptor &dst)
{
	std::string path = dst.getPathname();
	if (chmod(path.c_str(), src.getStat().st_mode) < 0) {
		std::cerr << "Error copy file mode: ";
		return false;
	}

	return true;
}
