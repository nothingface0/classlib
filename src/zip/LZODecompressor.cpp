#include "classlib/zip/LZODecompressor.h"
#include "classlib/zip/LZOError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <lzo/lzo1.h>
#include <lzo/lzo1a.h>
#include <lzo/lzo1b.h>
#include <lzo/lzo1c.h>
#include <lzo/lzo1f.h>
#include <lzo/lzo1x.h>
#include <lzo/lzo1y.h>
#include <lzo/lzo1z.h>
#include <lzo/lzo2a.h>

/// Call LZO compression function, with simple arguments.
#define LZO_CALL(rc, func, in, inlen, out, outlen) \
  if ((rc = func (in, inlen, out, outlen, 0)) != LZO_E_OK) \
    throwLZOError(#func, rc);

static void
throwLZOError (const char *label, int rc)
{
    throw lat::LZOError (label, rc);
}

namespace lat {

/** Create a new decompressor for the specified algorithm.

    @param algo        LZO algorithm the data was compressed with. */
LZODecompressor::LZODecompressor (Algorithm algo /* = LZO1X */)
    : m_algo (algo),
      m_input (0),
      m_len (0),
      m_in (0),
      m_out (0)
{
    reset ();
}

/** Close the decompressor.  */
LZODecompressor::~LZODecompressor (void)
{
    end ();
}

/** Set decompression algorithm.

    This function sets the decompression algorithm to @a algo.  The
    change will take effect the next time something is decompressed.  */
void
LZODecompressor::algorithm (Algorithm algo)
{
    m_algo = algo;
}

/** Get the compression algorithm, level and sublevel. */
LZOConstants::Algorithm
LZODecompressor::algorithm (void) const
{
    return m_algo;
}

/** Reset the decompression engine so that a new set of input can be
    processed.  */
void
LZODecompressor::reset (void)
{
    end ();

    int rc;
    if ((rc = lzo_init ()) != LZO_E_OK)
      throwLZOError ("lzo_init", rc);

    m_in = 0;
    m_out = 0;
}

/** Close the decompressor and discard any unprocessed input.

    This method should be called when the decompressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined, except it can be safely destructed.  */
void
LZODecompressor::end (void)
{
    m_input = 0;
    m_len = 0;
}

//////////////////////////////////////////////////////////////////////
/** Check if the decompressor needs more input data through input().

    Returns @c true if the decompressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
LZODecompressor::more (void) const
{
    // Request input if we don't have any.
    return m_len == 0;
}

/** Set input data from decompression.

    This method should be called when more() returns true indicating
    that the decompressor needs more input.  It should not be called
    if unprocessed input is still left for the engine to process.
    
    Note that LZO always decompresses all the data in one go. All data
    to decompress must be provided in a single input() call, and the
    decompressed output retrieved in a single decompress() call.  */
void
LZODecompressor::input (const void *buffer, IOSize length)
{
    ASSERT (buffer);
    ASSERT (! m_input);
    ASSERT (! m_len);
    m_input = buffer;
    m_len = length;
    m_in += length;
}

/** Return the number of bytes of unprocessed input still left in the
    input buffer.

    LZO decompresses entire compressed buffers, with no left-over data
    in input, so this method always returns zero. */
IOSize
LZODecompressor::rest (void) const
{
    return 0;
}

//////////////////////////////////////////////////////////////////////
/** Check to see if the end of compressed data stream has been reached.

    LZO decompresses entire compressed buffers. It cannot be used in
    stream mode. Hence this method always returns @c true as there
    never is a need to provide more input. */
bool
LZODecompressor::finished (void) const
{
    return true;
}

/** Fill the given buffer with decompressed data.

    LZO always decompresses entire buffers. The caller must ensure
    the buffer provided in the arguments is sufficiently large to
    absorb decompressed data from last input() call.

    @param into		The buffer to decompress into.
    @param length	The size of the buffer.

    @return The number of bytes of data written to @a into, which is
            less or equal to @a length.  The caller must ensure there
            is enough space in @a into for the entirety of output.

    @throw  LZOError in case of errors. */
IOSize
LZODecompressor::decompress (void *into, IOSize length)
{
    ASSERT (! length || into);

    // Decompress the input data.
    lzo_uint destlen = length;
    int rc;

    switch (m_algo)
    {
    case LZO1:
        LZO_CALL (rc, lzo1_decompress,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1A:
        LZO_CALL (rc, lzo1a_decompress,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1B:
        LZO_CALL (rc, lzo1b_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1C:
        LZO_CALL (rc, lzo1c_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1F:
        LZO_CALL (rc, lzo1f_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1X:
        LZO_CALL (rc, lzo1x_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1Y:
        LZO_CALL (rc, lzo1y_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO1Z:
        LZO_CALL (rc, lzo1z_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;

    case LZO2A:
        LZO_CALL (rc, lzo2a_decompress_safe,
		  (lzo_bytep) m_input, m_len,
		  (lzo_bytep) into, &destlen);
	break;
    }

    // Track how much compressed output we have produced.
    m_out += destlen;

    // Mark input all used up.
    m_input = 0;
    m_len = 0;

    // Return the size of decompressed output.
    return destlen;
}

/** Return the number of bytes of compressed input processed.  */
IOOffset
LZODecompressor::in (void) const
{ return m_in; }

/** Return the number of bytes of uncompressed output produced.  */
IOOffset
LZODecompressor::out (void) const
{ return m_out; }

} // namespace lat
