#ifndef _FILE_DESCRIPTOR_HPP	/* filedescriptor.hpp */
#define _FILE_DESCRIPTOR_HPP

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>

class FileDescriptor {
    private:
	const int default_perm = 0644;
	int32_t fd;
	struct stat st;
	std::string errmsg;
	std::string pathname;

    public:
	static const int READ = 0;
	static const int WRITE = 1;
	static const int BUFSIZE = 4096;

	bool Open(const std::string pathname, const int mode);
	std::string getErrmsg();
	std::string getPathname();
	int32_t getFd();
	struct stat &getStat();
	~FileDescriptor();
};

#endif	/* filedescriptor.hpp */
