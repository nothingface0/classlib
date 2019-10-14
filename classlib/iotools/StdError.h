#ifndef CLASSLIB_STD_ERROR_H
# define CLASSLIB_STD_ERROR_H
# include "classlib/iotools/IOException.h"

namespace lat {

/** An I/O-related error condition.  */
class StdError : public IOException
{
public:
    StdError (int code = 0, Error *next = 0);
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
#endif // CLASSLIB_STD_ERROR_H
