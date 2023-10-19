#ifndef CLASSLIB_LZO_COMPRESSOR_H
# define CLASSLIB_LZO_COMPRESSOR_H
# include "classlib/zip/Compressor.h"
# include "classlib/zip/LZOConstants.h"
# include <string>

namespace lat {

/** Compress output data using the LZO algorithm family. */
class LZOCompressor : public Compressor, public LZOConstants
{
public:
    LZOCompressor (unsigned level = LZO1X << 24 | 999 << 8 | 9,
		   bool optimise = true);
    ~LZOCompressor (void);

    // Global compressor state
    virtual void	level (unsigned level);
    virtual unsigned	level (void) const;
    virtual void	optimise (bool value);
    virtual bool	optimise (void) const;
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
    virtual IOSize      estimateWorstSize (void) const;

    // Statistics
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    unsigned		m_level;	//< LZO compression level.
    bool		m_optimise;	//< LZO post-processing enabled.
    std::string		m_buffered;	//< Remaining compressed data.
    std::string		m_optbuf;	//< Optimisation intermediate buffer.
    const void		*m_input;	//< Input data.
    IOSize		m_len;		//< Input length.
    IOOffset		m_in;		//< Total data input.
    IOOffset		m_out;		//< Total data output.

    // no semantics, undefined
    LZOCompressor (const LZOCompressor &other);
    LZOCompressor &operator= (const LZOCompressor &other);
};

} // namespace lat
#endif // CLASSLIB_LZO_COMPRESSOR_H
