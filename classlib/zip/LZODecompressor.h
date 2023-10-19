#ifndef CLASSLIB_LZO_DECOMPRESSOR_H
# define CLASSLIB_LZO_DECOMPRESSOR_H
# include "classlib/zip/Decompressor.h"
# include "classlib/zip/LZOConstants.h"

namespace lat {

/** Decompress input data using the LZO algorithm.

    The LZO algorithm only decompresses entire compressed data blocks
    at a time. The caller must know the size of the decompressed
    output in advance and pre-allocate a sufficiently large block of
    memory into which the decompression happens.

    Hence it is not possible to pass in data incrementally as the base
    class API describes. However as long as the caller provides full
    compressed input in one go and a sufficiently large output buffer,
    the class still works according to the base class interface. */
class LZODecompressor : public Decompressor, public LZOConstants
{
public:
    LZODecompressor (Algorithm algo = LZO1X);
    ~LZODecompressor (void);

    // Global decompressor state
    virtual void	algorithm (Algorithm algo);
    virtual Algorithm   algorithm (void) const;
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
    Algorithm           m_algo;		//< LZO algorithm.
    const void		*m_input;	//< Input data.
    IOSize		m_len;		//< Input length.
    IOOffset		m_in;		//< Total data input.
    IOOffset		m_out;		//< Total data output.

    // no semantics, undefined
    LZODecompressor (const LZODecompressor &other);
    LZODecompressor &operator= (const LZODecompressor &other);
};

} // namespace lat
#endif // CLASSLIB_LZO_DECOMPRESSOR_H
