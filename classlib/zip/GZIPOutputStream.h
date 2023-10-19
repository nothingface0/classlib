#ifndef CLASSLIB_GZIP_OUTPUT_STREAM_H
# define CLASSLIB_GZIP_OUTPUT_STREAM_H
# include "classlib/zip/ZOutputStream.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
class ZCompressor;

/** A stream filter to write GZIP-compressed data.  */
class GZIPOutputStream : public ZOutputStream
{
public:
    GZIPOutputStream (OutputStream	*output,
		      ZCompressor	*engine = 0,
		      IOSize		bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // implicit copy constructor (= none)
    // implicit assignment operator (= none)

    virtual void	finish (void);
};

} // namespace lat
#endif // CLASSLIB_GZIP_OUTPUT_STREAM_H
