#ifndef _COPY_HPP /* copy.hpp */
#define _COPY_HPP

#include "filedescriptor.hpp"

namespace copy
{
bool copy_file(FileDescriptor &src, FileDescriptor &dst);
bool copy_permissions(FileDescriptor &src_fd, FileDescriptor &dst_fd);
}

#endif /* copy.hpp */
