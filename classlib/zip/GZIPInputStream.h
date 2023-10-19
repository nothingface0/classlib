#ifndef CLASSLIB_GZIP_INPUT_STREAM_H
# define CLASSLIB_GZIP_INPUT_STREAM_H
# include "classlib/zip/ZInputStream.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
class ZDecompressor;

/** A stream filter to read GZIP-compressed data.  */
class GZIPInputStream : public ZInputStream, public ZConstants
{
public:
    GZIPInputStream (InputStream	*input,
		     ZDecompressor	*engine = 0,
		     IOSize		bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // implicit copy constructor (= none)
    // implicit assignment operator (= none)

    using ZInputStream::read;
    virtual IOSize	read (void *into, IOSize n);

private:
    bool		m_eof;
};

} // namespace lat
#endif // CLASSLIB_GZIP_INPUT_STREAM_H
