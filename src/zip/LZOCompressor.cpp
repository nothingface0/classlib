#include "classlib/zip/LZOCompressor.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/zip/LZOError.h"
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

/// Verify predicate @a x is true at compile time, in statement context.
#define STATIC_CHECK(x)                                                        \
  do {                                                                         \
    switch (0) {                                                               \
    case 0:                                                                    \
    case (x):;                                                                 \
    }                                                                          \
  } while (0)

/// Maximum possible data expansion during compression, as specified in the LZO
/// FAQ.
#define lzo_worst_compress(algo, x)                                            \
  (((algo) < LZO2A) ? ((x) + ((x) / 16) + 64 + 3) : ((x) + ((x) / 8) + 128 + 3))

/// Call LZO compression function, with simple arguments.
#define LZO_SCALL(rc, func, in, inlen, out, outlen, workmem)                   \
  if ((rc = func(in, inlen, out, outlen, workmem)) != LZO_E_OK)                \
    throwLZOError(#func, rc);

/// Call LZO compression function, with simple + level argument.
#define LZO_LCALL(rc, func, in, inlen, out, outlen, workmem, level)            \
  if ((rc = func(in, inlen, out, outlen, workmem, level)) != LZO_E_OK)         \
    throwLZOError(#func, rc);

/// Call LZO compression function, with dictionary and level arguments.
#define LZO_DCALL(rc, func, in, inlen, out, outlen, workmem, level)            \
  if ((rc = func(in, inlen, out, outlen, workmem, 0, 0, 0, level)) !=          \
      LZO_E_OK)                                                                \
    throwLZOError(#func, rc);

/// Call LZO optimisation function.
#define LZO_OPTCALL(opt, rc, func, out, outlen, inlen)                         \
  if (opt && outlen < inlen) {                                                 \
    lzo_uint newlen = inlen;                                                   \
    if (m_optbuf.size() < newlen)                                              \
      m_optbuf.resize(newlen, '\0');                                           \
    if ((rc = func(out, outlen, (lzo_bytep)&m_optbuf[0], &newlen, 0)) !=       \
            LZO_E_OK ||                                                        \
        newlen != inlen)                                                       \
      throwLZOError(#func, rc);                                                \
  }

static void throwLZOError(const char *label, int rc) {
  throw lat::LZOError(label, rc);
}

namespace lat {

/** Create a new compressor using the specified compression level.

    @param level	LZO compression algorithm and level parametres.
    @param optimise	Enable optimisation (LZO1X and LZO1Y algorithms).

    The level must be a combination of LZO compression algorithm choice
    and compression level and sublevel, in the form of (algorithm << 24)
    plus (level << 8) plus (sublevel). If the algorithm / level does not
    have sublevels, leave that part zero.

    The possible algorithms, levels and sublevels are:
    - LZO1: 1, 99.
    - LZO1A: 1, 99.
    - LZO1B: 1 - 9, 99, 999.
    - LZO1C: 1 - 9, 99, 999.
    - LZO1F: 1, 999.
    - LZO1X: 1, 1(11), 1(12), 1(15), 999(1 - 9).
    - LZO1Y: 1, 999(1 - 9).
    - LZO1Z: 999(1 - 9).
    - LZO2A: 999.

    @throw LZOError if LZO library initialisation fails.  */
LZOCompressor::LZOCompressor(unsigned level /* = LZO1X << 24 | 999 << 8 | 9 */,
                             bool optimise /* = true */)
    : m_level(level), m_optimise(optimise), m_input(0), m_len(0), m_in(0),
      m_out(0) {
  reset();
}

/** Close the compressor.  */
LZOCompressor::~LZOCompressor(void) { end(); }

/** Set current compression level.

    This function sets the current compression level to @a level.  The
    change will take effect the next time something is compressed.  */
void LZOCompressor::level(unsigned level) { m_level = level; }

/** Get the compression algorithm, level and sublevel. */
unsigned LZOCompressor::level(void) const { return m_level; }

/** Set current optimisation mode.

    This function sets the current optimisation to @a optimise.  The
    change will take effect the next time something is compressed.  */
void LZOCompressor::optimise(bool value) { m_optimise = value; }

/** Get the optimisation setting (LZO1X algorithms). */
bool LZOCompressor::optimise(void) const { return m_optimise; }

/** Reset the compression engine so that a new set of input can be
    processed. */
void LZOCompressor::reset(void) {
  end();

  int rc;
  if ((rc = lzo_init()) != LZO_E_OK)
    throwLZOError("lzo_init", rc);

  m_in = 0;
  m_out = 0;
}

/** Close the compressor and discard any unprocessed input.

    This method should be called when the compressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined, except it can be safely destructed. */
void LZOCompressor::end(void) {
  m_optbuf.clear();
  m_buffered.clear();
  m_input = 0;
  m_len = 0;
}

/** Check if the compressor needs more input data through input().

    Returns @c true if the compressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool LZOCompressor::more(void) const {
  // Request input if we a) don't have any, and b) caller has
  // consumed all outbound compressed data.
  return m_len == 0 && m_buffered.empty();
}

/** Set input data for compression.

    This method should be called when more() returns true indicating
    that the compressor needs more input.  It should not be called if
    unprocessed input is still left for the engine to process.

    Note that LZO compression always compresses all the data in one go.
    Ideally data is returned to caller in one go by compress() as well;
    if too small an output buffer is given then output will be buffered
    internally.  In that case more input is permitted only once all the
    compressed output has been consumed. */
void LZOCompressor::input(const void *buffer, IOSize length) {
  // Update input buffers.  This should be called only if we have
  // not yet been told to finish.  The buffer should be valid.
  ASSERT(buffer);
  ASSERT(!m_input);
  ASSERT(!m_len);
  ASSERT(m_buffered.empty());
  ASSERT(m_optbuf.empty());
  m_input = buffer;
  m_len = length;
  m_in += length;
}

/** Indicate that the compression should end with the current contents
    of the input buffer.  LZO always compresses in a single operation
    on compress() call, so this method is irrelevant. */
void LZOCompressor::finish(void) {}

/** Indicate that the output should be flushed to match to current
    contents of the input buffer.  LZO always compresses in a single
    operation on compress() call, so this method is irrelevant. */
void LZOCompressor::flush(void) {}

/** Check to see if the end of compressed output has been reached.
    Returns @c true whenever compress() has been called and all
    compressed data has been read back. */
bool LZOCompressor::finished(void) const {
  return m_len == 0 && m_buffered.empty();
}

/** Check to see if the current input has been flushed.  */
bool LZOCompressor::flushed(void) const {
  return m_len == 0 && m_buffered.empty();
}

/** Fill the given buffer with compressed data.

    Returns the number of bytes actually written into the buffer.  If
    the return value is zero, more() should be called to determine if
    more input needs to be given.

    LZO compression always compresses the entire input in one single
    operation on compress() call.  Ideally data is returned to caller
    in one go by compress() as well; the caller can ensure large enough
    of a receive buffer with estimateWorstSize() after calling input().
    If too small an output buffer is given on compress(), then the data
    will be buffered internally and returned piecemeal until no more
    compressed data is left.

    It is permitted to call this method with a null buffer but then the
    data will just be buffered internally.  It is permitted to read the
    output in any size of chunks that suits the application, reading in
    larger chunks allows the compressor to operate more efficiently; call
    estimateWorstSize() to determine buffer size sufficient for returning
    all current input in one go.  If the output buffer is large enough,
    no internal buffering will happen.

    Note that when optimisation is enabled, LZO will use the input data
    buffer given to compress() to perform the optimisation pass in-place.
    Hence the buffer will be garbled on output if optimisation is on.

    @param into		The buffer to compress into.
    @param length	The size of the buffer; at most this
                        many bytes will be written to @a into.

    @return The number of bytes of data written to @a into; if zero,
            call more() to find out if more input data is needed, or
            finished() to find out if the end of input stream has been
            reached.

    @throw  LZOError in case the compressor is stalled and no progress
            is possible, in case of an internal error, or if the level
            does not translate to any valid choice of LZO parametres. */

IOSize LZOCompressor::compress(void *into, IOSize length) {
  union lzo_workmem {
    char lzo1[LZO1_MEM_COMPRESS];
    char lzo1_99[LZO1_99_MEM_COMPRESS];
    char lzo1a[LZO1A_MEM_COMPRESS];
    char lzo1a_99[LZO1A_99_MEM_COMPRESS];
    char lzo1b[LZO1B_MEM_COMPRESS];
    char lzo1b_99[LZO1B_99_MEM_COMPRESS];
    char lzo1b_999[LZO1B_999_MEM_COMPRESS];
    char lzo1c[LZO1C_MEM_COMPRESS];
    char lzo1c_99[LZO1C_99_MEM_COMPRESS];
    char lzo1c_999[LZO1C_999_MEM_COMPRESS];
    char lzo1f_1[LZO1F_MEM_COMPRESS];
    char lzo1f_999[LZO1F_999_MEM_COMPRESS];
    char lzo1x_1[LZO1X_1_MEM_COMPRESS];
    char lzo1x_1_11[LZO1X_1_11_MEM_COMPRESS];
    char lzo1x_1_12[LZO1X_1_12_MEM_COMPRESS];
    char lzo1x_1_15[LZO1X_1_15_MEM_COMPRESS];
    char lzo1x_999[LZO1X_999_MEM_COMPRESS];
    char lzo1y_1[LZO1Y_MEM_COMPRESS];
    char lzo1y_999[LZO1Y_999_MEM_COMPRESS];
    char lzo1z_1[LZO1_MEM_COMPRESS];
    char lzo1z_999[LZO1Z_999_MEM_COMPRESS];
    char lzo2a_999[LZO2A_999_MEM_COMPRESS];
  };

  ASSERT(!length || into);

  // If we have buffered compressed data, return that.
  if (!m_buffered.empty()) {
    IOSize copyout = std::min<IOSize>(m_buffered.size(), length);
    memcpy(into, &m_buffered[0], copyout);
    m_buffered.erase(0, copyout);
    return copyout;
  }

  // No compressed data, do some real compression.
  lzo_uint destlen = lzo_worst_compress(m_level >> 24, m_len);
  lzo_workmem workmem;
  void *dest = into;
  int rc;

  if (length < destlen) {
    m_buffered.resize(destlen, '\0');
    dest = &m_buffered[0];
  }

  switch (m_level) {
  case LZO1 << 24 | 1 << 8:
    LZO_SCALL(rc, lzo1_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1 << 24 | 99 << 8:
    LZO_SCALL(rc, lzo1_99_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1A << 24 | 1 << 8:
    LZO_SCALL(rc, lzo1a_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1A << 24 | 99 << 8:
    LZO_SCALL(rc, lzo1a_99_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1B << 24 | 1 << 8:
  case LZO1B << 24 | 2 << 8:
  case LZO1B << 24 | 3 << 8:
  case LZO1B << 24 | 4 << 8:
  case LZO1B << 24 | 5 << 8:
  case LZO1B << 24 | 6 << 8:
  case LZO1B << 24 | 7 << 8:
  case LZO1B << 24 | 8 << 8:
  case LZO1B << 24 | 9 << 8:
    LZO_LCALL(rc, lzo1b_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem, (m_level >> 8) & 0xf);
    break;

  case LZO1B << 24 | 99 << 8:
    LZO_SCALL(rc, lzo1b_99_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1B << 24 | 999 << 8:
    LZO_SCALL(rc, lzo1b_999_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  case LZO1C << 24 | 1 << 8:
  case LZO1C << 24 | 2 << 8:
  case LZO1C << 24 | 3 << 8:
  case LZO1C << 24 | 4 << 8:
  case LZO1C << 24 | 5 << 8:
  case LZO1C << 24 | 6 << 8:
  case LZO1C << 24 | 7 << 8:
  case LZO1C << 24 | 8 << 8:
  case LZO1C << 24 | 9 << 8:
    LZO_LCALL(rc, lzo1c_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem, (m_level >> 8) & 0xf);
    break;

  case LZO1C << 24 | 99 << 8:
    LZO_SCALL(rc, lzo1c_99_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1C << 24 | 999 << 8:
    LZO_SCALL(rc, lzo1c_999_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  case LZO1F << 24 | 1 << 8:
    LZO_SCALL(rc, lzo1f_1_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1F << 24 | 999 << 8:
    LZO_SCALL(rc, lzo1f_999_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  case LZO1X << 24 | 1 << 8:
    LZO_SCALL(rc, lzo1x_1_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1X << 24 | 1 << 8 | 11:
    LZO_SCALL(rc, lzo1x_1_11_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  case LZO1X << 24 | 1 << 8 | 12:
    LZO_SCALL(rc, lzo1x_1_12_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  case LZO1X << 24 | 1 << 8 | 15:
    LZO_SCALL(rc, lzo1x_1_15_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  case LZO1X << 24 | 999 << 8 | 1:
  case LZO1X << 24 | 999 << 8 | 2:
  case LZO1X << 24 | 999 << 8 | 3:
  case LZO1X << 24 | 999 << 8 | 4:
  case LZO1X << 24 | 999 << 8 | 5:
  case LZO1X << 24 | 999 << 8 | 6:
  case LZO1X << 24 | 999 << 8 | 7:
  case LZO1X << 24 | 999 << 8 | 8:
  case LZO1X << 24 | 999 << 8 | 9:
    LZO_DCALL(rc, lzo1x_999_compress_level, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem, m_level & 0xf);
    LZO_OPTCALL(m_optimise, rc, lzo1x_optimize, (lzo_bytep)dest, destlen,
                m_len);
    break;

  case LZO1Y << 24 | 1 << 8:
    LZO_SCALL(rc, lzo1y_1_compress, (lzo_bytep)m_input, m_len, (lzo_bytep)dest,
              &destlen, &workmem);
    break;

  case LZO1Y << 24 | 999 << 8 | 1:
  case LZO1Y << 24 | 999 << 8 | 2:
  case LZO1Y << 24 | 999 << 8 | 3:
  case LZO1Y << 24 | 999 << 8 | 4:
  case LZO1Y << 24 | 999 << 8 | 5:
  case LZO1Y << 24 | 999 << 8 | 6:
  case LZO1Y << 24 | 999 << 8 | 7:
  case LZO1Y << 24 | 999 << 8 | 8:
  case LZO1Y << 24 | 999 << 8 | 9:
    LZO_DCALL(rc, lzo1y_999_compress_level, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem, m_level & 0xf);
    LZO_OPTCALL(m_optimise, rc, lzo1y_optimize, (lzo_bytep)dest, destlen,
                m_len);
    break;

  case LZO1Z << 24 | 999 << 8 | 1:
  case LZO1Z << 24 | 999 << 8 | 2:
  case LZO1Z << 24 | 999 << 8 | 3:
  case LZO1Z << 24 | 999 << 8 | 4:
  case LZO1Z << 24 | 999 << 8 | 5:
  case LZO1Z << 24 | 999 << 8 | 6:
  case LZO1Z << 24 | 999 << 8 | 7:
  case LZO1Z << 24 | 999 << 8 | 8:
  case LZO1Z << 24 | 999 << 8 | 9:
    LZO_DCALL(rc, lzo1z_999_compress_level, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem, m_level & 0xf);
    break;

  case LZO2A << 24 | 999 << 8:
    LZO_SCALL(rc, lzo2a_999_compress, (lzo_bytep)m_input, m_len,
              (lzo_bytep)dest, &destlen, &workmem);
    break;

  default:
    throwLZOError("unexpected compress level", LZO_E_NOT_YET_IMPLEMENTED);
  }

  // Track how much compressed output we have produced.
  m_out += destlen;

  // Mark input all used up.
  m_input = 0;
  m_len = 0;

  // Return the amount of compressed data produced.
  if (m_buffered.empty())
    return destlen;
  else {
    lzo_uint copyout = std::min(destlen, length);
    memcpy(into, &m_buffered[0], copyout);
    m_buffered.erase(0, copyout);
    return copyout;
  }
}

/** Return the worst possible compressed size of current input. */
IOSize LZOCompressor::estimateWorstSize(void) const {
  return lzo_worst_compress(m_level >> 24, m_len);
}

/** Return the number of bytes of uncompressed input processed.  */
IOOffset LZOCompressor::in(void) const { return m_in; }

/** Return the number of bytes of compressed output produced.  */
IOOffset LZOCompressor::out(void) const { return m_out; }

} // namespace lat
