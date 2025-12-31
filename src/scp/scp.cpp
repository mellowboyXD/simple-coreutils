/* scp -- simple copy utility
 * This is an attempt to recreate the main part of the `cp` command in UNIX like
 * operating systems.
 *
 * POSIX Compliant and utilizing only direct systems calls like 
 * `open/read/write/stat/close`
 *
 * Author: mellowboyxd */

#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

const size_t BUFF_SIZE = 4096;

void print_usage();
bool copy_file(std::string src, std::string dest);
bool copy_permissions(int src_fd, int dest_fd);

class Writer {
    private:
	int32_t fd;
	bool noErr;

    public:
	explicit Writer(const std::string &pathname)
	{
		fd = open(pathname.c_str(), O_WRONLY | O_CREAT, 0644);
		if (fd < 0) {
			std::cerr << "Error occured opening file: " << pathname
				  << std::endl;
			noErr = false;
			return;
		}
		noErr = true;
	}

	bool writel(const std::string &buf)
	{
		if (!noErr)
			return false;

		ssize_t count = buf.length();
		if ((write(fd, buf.c_str(), buf.length())) != count) {
			std::cerr << "Failed to write to file: " << buf;
			return false;
		}
		
		return true;
	}

	bool getNoErr()
	{
		return noErr;
	}

	~Writer()
	{
		if (noErr)
			close(fd);
	}
};

class Reader {
    private:
	int32_t fd;
	bool noErr;

    public:
	explicit Reader(std::string &pathname)
	{
		fd = open(pathname.c_str(), O_RDONLY);
		if (fd < 0) {
			std::cerr << "Error opening file for reading"
				  << std::endl;
			noErr = false;
			return;
		}
		noErr = true;
	}

	bool readl(void *buf)
	{
		int n;
		if ((n = read(fd, buf, BUFF_SIZE)) <= 0) {
			if (n == -1)
				std::cerr << "Failed to read lines from file"
					  << std::endl;
			return false;
		}
		return true;
	}

	bool getNoErr()
	{
		return noErr;
	}

	~Reader()
	{
		if (noErr)
			close(fd);
	}
};

int main(int argc, const char **argv)
{
	if (argc < 3) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	if (!copy_file(argv[1], argv[2]))
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}

void print_usage()
{
	std::cout << "Usage: scp <source> <destination>" << std::endl;
}

bool copy_file(std::string src, std::string dst)
{
	char buf[BUFF_SIZE];

	Reader fr = Reader(src);
	if (!fr.getNoErr())
		return false;

	Writer fw = Writer(dst);
	if (!fw.getNoErr())
		return false;

	while (fr.readl(buf)) {
		if (!fw.writel(buf))
			return false;
	}

	return true;
}
