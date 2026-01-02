#include "filedescriptor.hpp"

bool FileDescriptor::Open(const std::string pathname, const int mode)
{
	switch (mode) {
	case READ:
		if (stat(pathname.c_str(), &st) == -1) {
			errmsg = pathname + ": Permission denied";
			return false;
		}

		if (!S_ISREG(st.st_mode)) {
			if (S_ISDIR(st.st_mode)) {
				errmsg = pathname + ": Is a directory";
			} else {
				errmsg = pathname + ": Not a regular file";
			}
			return false;
		}

		fd = open(pathname.c_str(), O_RDONLY);
		if (fd == -1) {
			errmsg = "Error opening file for read";
			return false;
		}
		break;
	case WRITE:
		fd = open(pathname.c_str(), O_WRONLY | O_CREAT, default_perm);
		if (fd == -1) {
			errmsg = "Error opening file for write";
			return false;
		}
		break;
	default:
		fd = -1;
		errmsg = "Invalid mode";
		return false;
	}

	this->pathname = pathname;
	return true;
}

std::string FileDescriptor::getErrmsg()
{
	return errmsg;
}

std::string FileDescriptor::getPathname()
{
	return pathname;
}

int32_t FileDescriptor::getFd()
{
	return fd;
}

struct stat &FileDescriptor::getStat()
{
	return st;
}

FileDescriptor::~FileDescriptor()
{
	if (fd != -1)
		close(fd);
}
