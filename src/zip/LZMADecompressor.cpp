#include "classlib/zip/LZMADecompressor.h"
#include "classlib/zip/LZMAError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <lzma.h>

static const lzma_stream STREAM_INIT = LZMA_STREAM_INIT;

namespace lat {

/** Private type for actual stream state. */
struct LZMADecompressor::Stream : lzma_stream
{
    size_t	       m_dictSize;     //< Size of dictionary.
    lzma_filter        m_filters [2];
    lzma_options_lzma  m_opts;
    lzma_action        m_action;
    lzma_ret	       m_state;        //< Current decompression state.
};

/** Create a new decompressor.

    The decompression parameters will be determined automatically from
    the input stream.  If @a wrap, the stream is expected to have ZLIB
    wrapper; otherwise it is expected to be raw compressed data.  */
LZMADecompressor::LZMADecompressor (size_t dictionarySize /* = DefaultDictionarySize */)
    : m_stream (new Stream)
{
    memset (m_stream, 0, sizeof (Stream));
    *static_cast<lzma_stream *>(m_stream) = STREAM_INIT;
    m_stream->m_dictSize = dictionarySize;
    reset ();
}

/** Close the decompressor.  */
LZMADecompressor::~LZMADecompressor (void)
{
    end ();
    delete m_stream;
}

/** Set decompression dictionary size to @a value. */
void
LZMADecompressor::dictionarySize (size_t value)
{
    ASSERT (m_stream);
    m_stream->m_dictSize = value;
}

/** Get current decompression dictionary size. */
size_t
LZMADecompressor::dictionarySize (void) const
{
    ASSERT (m_stream);
    return m_stream->m_dictSize;
}

/** Reset the decompression engine so that a new set of input can be
    processed.  */
void
LZMADecompressor::reset (void)
{
    int rc;
    ASSERT (m_stream);
    memset (&m_stream->m_opts, 0, sizeof (m_stream->m_opts));
    m_stream->m_opts.dict_size = m_stream->m_dictSize;
    m_stream->m_opts.preset_dict = 0;
    m_stream->m_opts.preset_dict_size = 0;
    m_stream->m_opts.lc = DefaultLiteralContextBits;
    m_stream->m_opts.lp = DefaultLiteralPositionBits;
    m_stream->m_opts.pb = DefaultPositionBits;
    m_stream->m_opts.mode = LZMA_MODE_NORMAL;
    m_stream->m_opts.nice_len = DefaultNiceMatchLength;
    m_stream->m_opts.mf = LZMA_MF_BT4;
    m_stream->m_opts.depth = DefaultMaxSearchDepth;
    m_stream->m_filters[0].id = LZMA_FILTER_LZMA2;
    m_stream->m_filters[0].options = &m_stream->m_opts;
    m_stream->m_filters[1].id = LZMA_VLI_UNKNOWN;

    // Initialise LZMA.
    if ((rc = lzma_raw_decoder (m_stream, m_stream->m_filters)) != LZMA_OK)
	throw LZMAError ("lzma_raw_decoder", rc);

    // Initialise state machine.
    m_stream->m_state = LZMA_OK;
    m_stream->m_action = LZMA_RUN;
    m_stream->next_in = 0;
    m_stream->avail_in = 0;
    m_stream->next_out = 0;
    m_stream->avail_out = 0;
}

/** Close the decompressor and discard any unprocessed input.

    This method should be called when the decompressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined (except that it is of course safe to destruct
    it).  */
void
LZMADecompressor::end (void)
{
    // This eats the possible error produced by ZLIB.  This method is
    // called from the destructor, so it is not possible to throw an
    // exception from here.  At any rate the possible error code is
    // Z_STREAM_ERROR (m_stream was inconsistent) which Just Shouldn't
    // Happen.
    ASSERT (m_stream);
    lzma_end (m_stream);
}

/** Check if the decompressor needs more input data through input().

    Returns @c true if the decompressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
LZMADecompressor::more (void) const
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_state != LZMA_STREAM_END);

    // In addition for checking for space on the input side, check to
    // see if the last decompression filled the whole output buffer.
    // Don't request more input until we are left with space in the
    // output buffer.  This avoids us gobbling input faster than the
    // client is consuming decompressed output, and in particular, it
    // avoids overrunning input if the client consumes output slower
    // than we can be fed input.
    return (! m_stream->avail_in
	    && (! m_stream->next_out || m_stream->avail_out));
}

/** Set input data from decompression.

    This method should be called when more() returns true indicating
    that the decompressor needs more input.  It should not be called
    if unprocessed input is still left for the engine to process.
    
    Note that there is no way to tell how much more input is required
    to reach the end of the compressed data stream.  You'll just have
    to give some and then in the end ask how much was not used by the
    decompressor by calling unused().  */
void
LZMADecompressor::input (const void *buffer, IOSize length)
{
    ASSERT (buffer);
    ASSERT (m_stream);
    ASSERT (m_stream->m_state == LZMA_OK || m_stream->m_state == LZMA_DATA_ERROR);
    m_stream->next_in = (unsigned char *) buffer; // cast away const, to char
    m_stream->avail_in = length;
}

/** Return the number of bytes of unprocessed input still left in the
    input buffer.

    Call this method after finished() returns @c true to determine how
    much input was left in the current input buffer unprocessed by the
    decompressor.  The return value is the number of bytes left in the
    input buffer (at the end of the buffer) last given to input().  */
IOSize
LZMADecompressor::rest (void) const
{
    ASSERT (m_stream);
    return m_stream->avail_in;
}

/** Check to see if the end of compressed data stream has been reached.

    The decompressor determines itself when it has reached the end of
    the compressed stream.  Call this method to find out when that has
    happened; until the method returns @c true, keep on providing more
    input.  Once the method returns @c true, you can use rest() to
    determine how much data the decompressor did not use from the last
    input buffer. */
bool
LZMADecompressor::finished (void) const
{
    ASSERT (m_stream);
    return m_stream->m_state == LZMA_STREAM_END;
}

/** Fill the given buffer with decompressed data.

    Returns the number of bytes actually written into the buffer.  If
    the return value is zero, more() should be called to determine if
    more input needs to be give.

    It is quite possible -- if not even normal -- for the decompressor
    to give no output while consuming some amounts of input, or to
    produce large amounts of output from little or no input.  It may
    also happen that the decompressor does not provide any output yet
    does not consume all available input; just call this method again.

    Zero return value does not necessarily mean that the produced
    output matches the input consumed so far.  This is so only when
    input has been provided up to a flush point and all input has been
    used.  Otherwise input may be gobbled up inside the decompression
    engine.

    It is permitted to call this method with a null buffer, though at
    some point the decompressor will stall without output space.  The
    client can read the output in any size of chunks suitable to it,
    reading in larger chunks allows the decompressor to operate more
    efficiently.  The entire output for the current input can be read
    in one go provided the output buffer is large enough.

    @param into		The buffer to decompress into.
    @param length	The size of the buffer; at most this
    			many bytes will be written to @a into.

    @return The number of bytes of data written to @a into; if zero,
	    call more() to find out if more input data is needed, or
	    finished() to find out if the end of input stream has been
	    reached.

    @throw  LZMAError if the input data is corrupted, not enough memory
            is available for decompression (the memory required can
            only be determined from the data), or in case of an
            internal error such as incorrect state machine state or
            corrupted stream object.  If input data is corrupted, the
            client can call sync() until the invalid data has been
            skipped and a good compression block is found again.  */
IOSize
LZMADecompressor::decompress (void *into, IOSize length)
{
    ASSERT (m_stream);
    ASSERT (! length || into);

    // Compress some data
    m_stream->next_out = (unsigned char *) into;
    m_stream->avail_out = length;
    switch (m_stream->m_state = lzma_code (m_stream, m_stream->m_action))
    {
    case LZMA_OK:		// Action completed, no change
    case LZMA_STREAM_END:	// End of stream
	break;

    case LZMA_BUF_ERROR:
	// No progress was possible due to lack of buffer space.  This
	// is caused mainly because our input optimisation logic in
	// #input() tends to withhold too much data from ZLIB.  (This
	// may happen also if `length' is zero, but that is trivial.)
	// Return zero to our caller to indicate that we need more
	// input data or a proper output buffer.
	m_stream->m_state = LZMA_OK;
	m_stream->avail_out = 1;
	return 0;

    default:
	// Oops. An error occurred.
	throw LZMAError ("lzma_code", m_stream->m_state);
    }

    // Return the amount of compressed data produced; if zero and
    // !`finished()', we should be given more input data (see also
    // `more()').
    return length - m_stream->avail_out;
}

/** Return the number of bytes of compressed input processed.  */
IOOffset
LZMADecompressor::in (void) const
{ ASSERT (m_stream); return m_stream->total_in; }

/** Return the number of bytes of uncompressed output produced.  */
IOOffset
LZMADecompressor::out (void) const
{ ASSERT (m_stream); return m_stream->total_out; }

} // namespace lat
