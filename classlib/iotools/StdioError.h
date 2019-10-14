#ifndef CLASSLIB_STDIO_ERROR_H
# define CLASSLIB_STDIO_ERROR_H
# include "classlib/iotools/IOException.h"

namespace lat {

/** An I/O-related error condition.  */
class StdioError : public IOException
{
public:
    StdioError (int code = 0, Error *next = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    int			m_code;
};

} // namespace lat
#endif // CLASSLIB_STDIO_ERROR_H
