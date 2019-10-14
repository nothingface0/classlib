#include "classlib/zip/ZInputStream.h"
#include "classlib/zip/ZDecompressor.h"
#include "classlib/zip/CRC32.h"

namespace lat {

/** FIXME */
ZInputStream::ZInputStream (InputStream		*input,
			    ZDecompressor	*engine /* = 0 */,
			    IOSize		bufsize /* = DEFAULT_BUF_SIZE */)
    : DecompressInputStream (engine ? engine : new ZDecompressor,
			     new CRC32, input, bufsize)
{}

ZInputStream::ZInputStream (InputStream		*input,
			    ZDecompressor	*engine,
			    Checksum		*checksum,
			    IOSize		bufsize /* = DEFAULT_BUF_SIZE */)
    : DecompressInputStream (engine ? engine : new ZDecompressor,
			     checksum ? checksum : new CRC32, input, bufsize)
{}

} // namespace lat
