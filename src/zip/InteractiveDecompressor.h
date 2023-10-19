#ifndef CLASSLIB_INTERACTIVE_DECOMPRESSOR_H
# define CLASSLIB_INTERACTIVE_DECOMPRESSOR_H
# include "classlib/sysapi/system.h"
# include "classlib/sysapi/IOTypes.h"

namespace lat {
class Decompressor;
class InputStream;
class OutputStream;

/** Non-blocking, incremental and interactive decompressor front-end.

    This class can be used to run a @Decompressor on non-blocking
    input and/or output or when the @Decompressor should be ran
    interactively on limited amounts of data at a time.

    This class processes both input and output in chunks, the upper
    limits of the chunk sizes for both input and output are given as
    constructor arguments.  At most one full chunk of input or output
    data is processed at a time even if the streams could handle more.

    A @Decompressor can consume arbitrary amounts of input before
    producing any output, and even a small amount of input can produce
    large amounts of output.  This class guarantees the partitioning
    of the input and output to at most the chunk sizes mentioned
    above, while at the same time remaining responsive to the maximum
    bandwiths of the input and output streams -- within the limits of
    the @Decompressor's abilities.

    This class must be inherted from to implement the input and output
    notification scheme.  On call to waitForInput() doInput() should
    be registered to handle the input becoming readable.  Similarly on
    call to waitForOutput() doOutput() should be registered to handle
    the output becoming writable.  */
class InteractiveDecompressor
{
public:
    enum { DEFAULT_BUF_SIZE = 4096 };

    virtual ~InteractiveDecompressor (void);

    virtual void	begin (void);

protected:
    InteractiveDecompressor (Decompressor	*decompressor,
			     InputStream	*input = 0,
			     OutputStream	*output = 0,
			     IOSize		inBufSize = DEFAULT_BUF_SIZE,
			     IOSize		outBufSize = DEFAULT_BUF_SIZE);

    virtual void	setInputStream (InputStream *input);
    virtual void	setOutputStream (OutputStream *output);

    virtual void	waitForInput (void);
    virtual void	waitForOutput (void);

    virtual void	doInput (void);
    virtual void	doOutput (void);
    virtual void	decompress (void);

    virtual bool	flush (void);
    virtual void	finish (void);

    Decompressor	*m_decompressor;//< The decompressor engine

    InputStream		*m_input;	//< The non-blocking input
    unsigned char	*m_inBuf;	//< Internal input buffer
    IOSize		m_inBufSize;	//< Size of m_inBuf
    bool		m_inputting;	//< Set when doInput() is registered

    OutputStream	*m_output;	//< The non-blocking output
    unsigned char	*m_outBuf;	//< Internal output buffer
    IOSize		m_outBufSize;	//< Size of m_outBuf
    IOSize		m_out;		//< Bytes of data remaining in m_outBuf
    IOSize		m_bottom;	//< Offset at which m_out begins
    bool		m_outputting;	//< Set when doOutput() is registered

private:
    // no semantics, undefined
    InteractiveDecompressor (const InteractiveDecompressor &);
    InteractiveDecompressor &operator= (const InteractiveDecompressor &);
};

} // namespace lat
#endif // CLASSLIB_INTERACTIVE_DECOMPRESSOR_H
