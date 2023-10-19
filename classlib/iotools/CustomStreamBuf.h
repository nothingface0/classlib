#ifndef CLASSLIB_CUSTOM_STREAM_BUF_H
# define CLASSLIB_CUSTOM_STREAM_BUF_H
# include "classlib/sysapi/system.h"
# include <streambuf>

namespace lat {
class CustomStreamBuf : public std::streambuf
{
public:
    typedef std::streamsize		size_type;
    typedef std::streamoff		offset_type;
    typedef std::streampos		pos_type;
    typedef std::ios_base::seekdir	seekdir;
    typedef std::ios_base::openmode	openmode;
};
} // namespace lat
#endif // CLASSLIB_CUSTOM_STREAM_BUF_H
