#ifndef CLASSLIB_LZMA_COMPRESSOR_H
# define CLASSLIB_LZMA_COMPRESSOR_H
# include "classlib/zip/Compressor.h"
# include "classlib/zip/LZMAConstants.h"

namespace lat {

/** Compress output data using the LZMA algorithm, also known as XZ.  */
class LZMACompressor : public Compressor, public LZMAConstants
{
public:
    // nowrap -> no zlib header, checksum
    LZMACompressor (unsigned preset = DEFAULT_COMPRESSION, bool extreme = false);
    ~LZMACompressor (void);

    // Global compressor state
    virtual unsigned	preset (void) const;
    virtual void	preset (unsigned level);
    virtual bool	extreme (void) const;
    virtual void	extreme (bool value);
    virtual size_t	memoryUsageLimit (void) const;
    virtual void	memoryUsageLimit (size_t value);
    virtual void	removeFilters (void);
    virtual void	filterLZMA (unsigned preset, bool extreme = false);
    virtual void	filterLZMA (size_t dictionarySize,
				    Mode mode,
				    MatchFinder matchFinder,
				    unsigned literalContextBits,
				    unsigned literalPositionBits,
				    unsigned positionBits,
				    unsigned niceMatchLength,
				    unsigned maxSearchDepth);
    virtual void	filterArch (Arch arch, unsigned offset = 0);
    virtual void	filterDelta (unsigned distance);
    virtual void	reset (void);
    virtual void	end (void);

    // Input management
    virtual bool	more (void) const;
    virtual void	input (const void *buffer, IOSize length);
    virtual void	finish (void);
    virtual void	flush (void);

    // Compression
    virtual bool	finished (void) const;
    virtual bool	flushed (void) const;
    virtual IOSize	compress (void *into, IOSize length);

    // Statistics
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    struct Stream;
    Stream		*m_stream;	//< LZMA state, null if not initialised

    // no semantics, undefined
    LZMACompressor (const LZMACompressor &other);
    LZMACompressor &operator= (const LZMACompressor &other);
};

} // namespace lat
#endif // CLASSLIB_LZMA_COMPRESSOR_H
