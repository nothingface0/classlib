#ifndef CLASSLIB_BZIP_OUTPUT_STREAM_H
# define CLASSLIB_BZIP_OUTPUT_STREAM_H
# include "classlib/zip/CompressOutputStream.h"
# include "classlib/zip/BZConstants.h"

namespace lat {
class BZCompressor;

/** A stream filter to write BZLIB-compressed data.  */
class BZIPOutputStream : public CompressOutputStream, public BZConstants
{
public:
    BZIPOutputStream (OutputStream	*output,
		      BZCompressor	*engine = 0,
		      IOSize		bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // no copy constructor
    // no assignment operator
};

} // namespace lat
#endif // CLASSLIB_BZIP_OUTPUT_STREAM_H
