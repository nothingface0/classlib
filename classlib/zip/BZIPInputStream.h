#ifndef CLASSLIB_BZIP_INPUT_STREAM_H
#define CLASSLIB_BZIP_INPUT_STREAM_H
#include "classlib/zip/BZConstants.h"
#include "classlib/zip/DecompressInputStream.h"

namespace lat {
class BZDecompressor;

/** A stream filter to read BZLIB-compressed data.  */
class BZIPInputStream : public DecompressInputStream, public BZConstants {
public:
  BZIPInputStream(InputStream *input, BZDecompressor *engine = 0,
                  IOSize bufsize = DEFAULT_BUF_SIZE);
  // implicit destructor
  // no copy constructor
  // no assignment operator
};

} // namespace lat
#endif // CLASSLIB_BZIP_INPUT_STREAM_H
