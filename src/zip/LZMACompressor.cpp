#include "classlib/zip/LZMACompressor.h"
#include "classlib/zip/LZMAError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <lzma.h>

static const lzma_stream STREAM_INIT = LZMA_STREAM_INIT;

static const lzma_match_finder MFVAL[] = {
    /* MFHC3 */ LZMA_MF_HC3,
    /* MFHC4 */ LZMA_MF_HC4,
    /* MFBT2 */ LZMA_MF_BT2,
    /* MFBT3 */ LZMA_MF_BT3,
    /* MFBT4 */ LZMA_MF_BT4
};

static const lzma_vli ARCHVAL[] = {
    /* ArchX86 */       LZMA_FILTER_X86,
    /* ArchPowerPC */   LZMA_FILTER_POWERPC,
    /* ArchIA64 */      LZMA_FILTER_IA64,
    /* ArchARM */       LZMA_FILTER_ARM,
    /* ArchARMThumb */  LZMA_FILTER_ARMTHUMB,
    /* ArchSPARC */     LZMA_FILTER_SPARC
};

namespace lat {
const size_t                      LZMAConstants::DefaultDictionarySize = LZMA_DICT_SIZE_DEFAULT;
const LZMAConstants::MatchFinder  LZMAConstants::DefaultMatchFinder = LZMAConstants::MFBT4;
const unsigned                    LZMAConstants::DefaultLiteralContextBits = LZMA_LC_DEFAULT;
const unsigned                    LZMAConstants::DefaultLiteralPositionBits = LZMA_LP_DEFAULT;
const unsigned                    LZMAConstants::DefaultPositionBits = LZMA_PB_DEFAULT;
const unsigned                    LZMAConstants::DefaultNiceMatchLength = 64;
const unsigned                    LZMAConstants::DefaultMaxSearchDepth = 0;

/** Private type for actual stream state. */
struct LZMACompressor::Stream : lzma_stream
{
    lzma_options_lzma  m_optsLZMA;
    lzma_options_bcj   m_optsBCJ;
    lzma_options_delta m_optsDelta;
    size_t             m_preset;
    size_t	       m_memlimit;
    size_t             m_numFilters;
    lzma_filter        m_filters [LZMA_FILTERS_MAX + 1];
    lzma_action        m_action;
    lzma_ret	       m_state;
    unsigned	       m_optsLZMAUsed : 1;
    unsigned	       m_optsBCJUsed : 1;
    unsigned	       m_optsDeltaUsed : 1;
};


/** Create a new compressor using the specified compression level.

    @param preset       Compression preset level.
    @param extreme      Flag to request extreme compression.

    @throw LZMAError if LZMA initialisation fails.  */
LZMACompressor::LZMACompressor (unsigned preset /* = DEFAULT_COMPRESSION */,
			        bool extreme /* = false */)
    : m_stream (new Stream)
{
    memset (m_stream, 0, sizeof (Stream));
    *static_cast<lzma_stream *>(m_stream) = STREAM_INIT;
    m_stream->m_preset = (preset | (extreme ? LZMA_PRESET_EXTREME : 0));
    reset ();
}

/** Close the compressor.  */
LZMACompressor::~LZMACompressor (void)
{
    end ();
    delete m_stream;
}

/** Set current compression level using @a preset.

    The change will take effect at next reset() call.  */
void
LZMACompressor::preset (unsigned value)
{
    ASSERT (m_stream);
    ASSERT (value <= BEST_COMPRESSION);
    m_stream->m_preset = value | (m_stream->m_preset & ~LZMA_PRESET_LEVEL_MASK);
    removeFilters ();
}

/** Get current compression level.  */
unsigned
LZMACompressor::preset (void) const
{
    ASSERT (m_stream);
    return m_stream->m_preset & LZMA_PRESET_LEVEL_MASK;
}

bool
LZMACompressor::extreme (void) const
{
    ASSERT (m_stream);
    return m_stream->m_preset & LZMA_PRESET_EXTREME;
}

void
LZMACompressor::extreme (bool value)
{
    ASSERT (m_stream);
    m_stream->m_preset = ((m_stream->m_preset & LZMA_PRESET_LEVEL_MASK)
			  | (value ? LZMA_PRESET_EXTREME : 0));
    removeFilters ();
}

size_t
LZMACompressor::memoryUsageLimit (void) const
{
    ASSERT (m_stream);
    return m_stream->m_memlimit;
}

void
LZMACompressor::memoryUsageLimit (size_t value)
{
    ASSERT (m_stream);
    m_stream->m_memlimit = value;
}

void
LZMACompressor::removeFilters (void)
{
    ASSERT (m_stream);
    m_stream->m_numFilters = 0;
    m_stream->m_optsLZMAUsed = 0;
    m_stream->m_optsBCJUsed = 0;
    m_stream->m_optsDeltaUsed = 0;
}

void
LZMACompressor::filterLZMA (unsigned preset, bool extreme /* = false */)
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_numFilters < LZMA_FILTERS_MAX);
    ASSERT (! m_stream->m_optsLZMAUsed);
    size_t ix = m_stream->m_numFilters++;
    int rc;

    m_stream->m_preset = (preset | (extreme ? LZMA_PRESET_EXTREME : 0));
    if ((rc = lzma_lzma_preset (&m_stream->m_optsLZMA, m_stream->m_preset)))
	throw LZMAError ("lzma_lzma_preset", rc);

    m_stream->m_filters[ix].id = LZMA_FILTER_LZMA2;
    m_stream->m_filters[ix].options = &m_stream->m_optsLZMA;
    m_stream->m_optsLZMAUsed = 1;
}

void
LZMACompressor::filterLZMA (size_t dictionarySize,
			    Mode mode,
			    MatchFinder matchFinder,
			    unsigned literalContextBits,
			    unsigned literalPositionBits,
			    unsigned positionBits,
			    unsigned niceMatchLength,
			    unsigned maxSearchDepth)
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_numFilters < LZMA_FILTERS_MAX);
    ASSERT (! m_stream->m_optsLZMAUsed);
    size_t ix = m_stream->m_numFilters++;
    memset (&m_stream->m_optsLZMA, 0, sizeof (m_stream->m_optsLZMA));
    m_stream->m_optsLZMA.dict_size = dictionarySize;
    m_stream->m_optsLZMA.preset_dict = 0;
    m_stream->m_optsLZMA.preset_dict_size = 0;
    m_stream->m_optsLZMA.lc = literalContextBits;
    m_stream->m_optsLZMA.lp = literalPositionBits;
    m_stream->m_optsLZMA.pb = positionBits;
    m_stream->m_optsLZMA.mode = (mode == ModeNormal ? LZMA_MODE_NORMAL : LZMA_MODE_FAST);
    m_stream->m_optsLZMA.nice_len = niceMatchLength;
    m_stream->m_optsLZMA.mf = MFVAL[matchFinder];
    m_stream->m_optsLZMA.depth = maxSearchDepth;
    m_stream->m_filters[ix].id = LZMA_FILTER_LZMA2;
    m_stream->m_filters[ix].options = &m_stream->m_optsLZMA;
    m_stream->m_optsLZMAUsed = 1;
}

void
LZMACompressor::filterArch (Arch arch, unsigned offset /* = 0 */)
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_numFilters < LZMA_FILTERS_MAX);
    ASSERT (! m_stream->m_optsBCJUsed);
    size_t ix = m_stream->m_numFilters++;
    memset (&m_stream->m_optsBCJ, 0, sizeof (m_stream->m_optsBCJ));
    m_stream->m_optsBCJ.start_offset = offset;
    m_stream->m_filters[ix].id = ARCHVAL[arch];
    m_stream->m_filters[ix].options = &m_stream->m_optsBCJ;
    m_stream->m_optsBCJUsed = 1;
}

void
LZMACompressor::filterDelta (unsigned distance)
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_numFilters < LZMA_FILTERS_MAX);
    ASSERT (! m_stream->m_optsDeltaUsed);
    size_t ix = m_stream->m_numFilters++;
    memset (&m_stream->m_optsDelta, 0, sizeof (m_stream->m_optsDelta));
    m_stream->m_optsDelta.type = LZMA_DELTA_TYPE_BYTE;
    m_stream->m_optsDelta.dist = distance;
    m_stream->m_filters[ix].id = LZMA_FILTER_DELTA;
    m_stream->m_filters[ix].options = &m_stream->m_optsDelta;
    m_stream->m_optsDeltaUsed = 1;
}

/** Reset the compression engine so that a new set of input can be
    processed.

    This is the only time new compression settings will take effect.

    @throw  LZMAError if ZLIB initialisation fails.  */
void
LZMACompressor::reset (void)
{
    int rc;
    ASSERT (m_stream);
    if (! m_stream->m_numFilters)
        filterLZMA (preset(), extreme());

    ASSERT (m_stream->m_optsLZMAUsed);
    ASSERT (m_stream->m_numFilters < LZMA_FILTERS_MAX);
    size_t ix = m_stream->m_numFilters++;
    m_stream->m_filters[ix].id = LZMA_VLI_UNKNOWN;

    // Initialise LZMA.
    if ((rc = lzma_raw_encoder (m_stream, m_stream->m_filters)) != LZMA_OK)
	throw LZMAError ("lzma_raw_encoder", rc);

    // Initialise state machine.
    m_stream->m_state = LZMA_OK;
    m_stream->m_action = LZMA_RUN;
    m_stream->next_in = 0;
    m_stream->avail_in = 0;
    m_stream->next_out = 0;
    m_stream->avail_out = 0;
}

/** Close the compressor and discard any unprocessed input.

    This method should be called when the compressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined (except that it is of course safe to destruct
    it).  */
void
LZMACompressor::end (void)
{
    // This eats the possible error produced by ZLIB.  This method is
    // called from the destructor, so it is not possible to throw an
    // exception from here.  At any rate the possible error codes are
    // Z_DATA_ERROR (input or output was discarded) or Z_STREAM_ERROR
    // (m_stream was inconsistent).  The former we can't do much about
    // since it needs to be possible to destruct the object in the
    // middle of compression, and the latter Just Shouldn't Happen.
    ASSERT (m_stream);
    lzma_end (m_stream);
}

//////////////////////////////////////////////////////////////////////
/** Check if the compressor needs more input data through input().

    Returns @c true if the compressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
LZMACompressor::more (void) const
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_state != LZMA_STREAM_END);

    // In addition for checking for space on the input side, check to
    // see if the last compression filled the whole output buffer.
    // Don't request more input until we are left with space in the
    // output buffer.  This avoids us gobbling input faster than the
    // client is consuming compressed output, and in particular, it
    // avoids overrunning input if the client consumes output slower
    // than we can be fed input.
    return (m_stream->m_action == LZMA_RUN
	    && ! m_stream->avail_in
	    && (! m_stream->next_out || m_stream->avail_out));
}

/** Set input data for compression.

    This method should be called when more() returns true indicating
    that the compressor needs more input.  It should not be called if
    unprocessed input is still left for the engine to process.  */
void
LZMACompressor::input (const void *buffer, IOSize length)
{
    // Update input buffers.  This should be called only if we have
    // not yet been told to finish.  The buffer should be valid.
    ASSERT (buffer);
    ASSERT (m_stream);
    ASSERT (m_stream->m_state == LZMA_OK);
    ASSERT (m_stream->m_action == LZMA_RUN);
    m_stream->next_in = (unsigned char *) buffer; // cast away const, to char
    m_stream->avail_in = length;
}

/** Indicate that the compression should end with the current contents
    of the input buffer.

    Call this function when you have given the compressor last input
    with input().  This will tell the compressor to start flushing its
    internal state out and make all the remaining output available.  */
void
LZMACompressor::finish (void)
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_action == LZMA_RUN);
    m_stream->m_action = LZMA_FINISH;
}

/** Indicate that the output should be flushed to match to current
    contents of the input buffer.

    This method tells the compressor that it should not accept any
    more input until the internal state has been flushed such that
    output matches the currently available input.  Call flushed() to
    find out when the output is complete.  Once it returns @c true,
    more input can be provided with input() again.

    Call this function when you need a precise synchronisation point
    in the middle of the compressed output.  This can be useful for
    error control purposes for instance.  Flushing degrades the
    quality of the compression however, so use it sparingly.

    This method cannot be called after finish() or while another flush
    is active.  */
void
LZMACompressor::flush (void)
{
    ASSERT (m_stream);
    ASSERT (m_stream->m_action == LZMA_RUN);
    ASSERT (m_stream->m_state == LZMA_OK);
    m_stream->m_action = LZMA_SYNC_FLUSH;
}

//////////////////////////////////////////////////////////////////////
/** Check to see if the end of compressed output has been reached.  */
bool
LZMACompressor::finished (void) const
{
    ASSERT (m_stream);
    return m_stream->m_state == LZMA_STREAM_END;
}

/** Check to see if the current input has been flushed.  */
bool
LZMACompressor::flushed (void) const
{
    ASSERT (m_stream);
    return (m_stream->m_state == LZMA_OK
            && m_stream->m_action != LZMA_SYNC_FLUSH);
}

/** Fill the given buffer with compressed data.

    Returns the number of bytes actually written into the buffer.  If
    the return value is zero, more() should be called to determine if
    more input needs to be given.

    It is quite possible -- if not even normal -- for the compressor
    to give no output while consuming large amounts of input, or to
    produce large amounts of output while consuming little or not
    input.  It may also happen that the compressor does not provide
    any output yet does not consume all available input; just call
    this method again (FIXME: a possibility of the state machine or a
    reality?).

    Zero return value does not necessarily mean that the produced
    output matches the input consumed so far.  This is so only when
    flushed() returns @c true, otherwise input may be gobbled up
    inside the compression engine.

    It is permitted to call this method with a null buffer, though at
    some point the compressor will stall without output space.  It is
    permitted to read the output in any size of chunks that suits the
    application, reading in larger chunks allows the compressor to
    operate more efficiently.  The entire output for the current input
    can be read in one operation provided the output buffer is large
    enough.

    @param into		The buffer to compress into.
    @param length	The size of the buffer; at most this
    			many bytes will be written to @a into.

    @return The number of bytes of data written to @a into; if zero,
	    call more() to find out if more input data is needed, or
	    finished() to find out if the end of input stream has been
	    reached.

    @throw  LZMAError in case the compressor is stalled and no progress
            is possible (error code will Z_BUF_ERROR), or in case of
	    an internal error (such as incorrect compression state
            machine state or corrupted stream).  */
IOSize
LZMACompressor::compress (void *into, IOSize length)
{
    ASSERT (m_stream);
    ASSERT (! length || into);
    ASSERT (m_stream->m_state == LZMA_OK);

    // Compress some data
    m_stream->next_out = (unsigned char *) into;
    m_stream->avail_out = length;
    switch (m_stream->m_state = lzma_code (m_stream, m_stream->m_action))
    {
    case LZMA_OK:
	// If the action completed, reset back to run (from flush);
	// otherwise retain it for the next time around.
	if (m_stream->avail_out != 0)
	    m_stream->m_action = LZMA_RUN;
	break;

    case LZMA_STREAM_END:
	// Finish is now complete.
	ASSERT (m_stream->m_action == LZMA_FINISH);
	m_stream->m_action = LZMA_RUN;
	break;
	
    case LZMA_BUF_ERROR:
	// No progress was possible due to lack of buffer space.
	// Return zero to our caller and indicate that we need more
	// input data and/or a proper output buffer.
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

/** Return the number of bytes of uncompressed input processed.  */
IOOffset
LZMACompressor::in (void) const
{ ASSERT (m_stream); return m_stream->total_in; }

/** Return the number of bytes of compressed output produced.  */
IOOffset
LZMACompressor::out (void) const
{ ASSERT (m_stream); return m_stream->total_out; }

} // namespace lat
