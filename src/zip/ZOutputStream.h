#ifndef CLASSLIB_Z_OUTPUT_STREAM_H
# define CLASSLIB_Z_OUTPUT_STREAM_H
# include "classlib/zip/CompressOutputStream.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
class ZCompressor;

/** Compress output data using the ZLIB algorithm.  */
class ZOutputStream : public CompressOutputStream, public ZConstants
{
public:
    ZOutputStream (OutputStream	*output,
		   ZCompressor	*engine = 0,
		   IOSize	bufsize = DEFAULT_BUF_SIZE);
    ZOutputStream (OutputStream	*output,
		   ZCompressor	*engine,
		   Checksum	*checksum,
		   IOSize	bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // implicit copy constructor (= none)
    // implicit assignment operator (= none)
};

} // namespace lat
#endif // CLASSLIB_Z_OUTPUT_STREAM_H
