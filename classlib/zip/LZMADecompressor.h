#ifndef CLASSLIB_LZMA_DECOMPRESSOR_H
# define CLASSLIB_LZMA_DECOMPRESSOR_H
# include "classlib/zip/Decompressor.h"
# include "classlib/zip/LZMAConstants.h"

namespace lat {

/** Decompress input data using the LZMA algorithm, also known as XZ.  */
class LZMADecompressor : public Decompressor, public LZMAConstants
{
public:
    LZMADecompressor (size_t dictionarySize = DefaultDictionarySize);
    ~LZMADecompressor (void);

    // Global decompressor state
    virtual size_t      dictionarySize (void) const;
    virtual void	dictionarySize (size_t size);
    virtual void	reset (void);
    virtual void	end (void);

    // Input management
    virtual bool	more (void) const;
    virtual void	input (const void *buffer, IOSize length);
    virtual IOSize	rest (void) const;

    // Decompression
    virtual bool	finished (void) const;
    virtual IOSize	decompress (void *into, IOSize length);

    // Statistics
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    struct Stream;
    Stream		*m_stream;	//< LZMA state, null if not initialised

    // no semantics, undefined
    LZMADecompressor (const LZMADecompressor &other);
    LZMADecompressor &operator= (const LZMADecompressor &other);
};

} // namespace lat
#endif // CLASSLIB_LZMA_DECOMPRESSOR_H
