#ifndef CLASSLIB_IO_ERROR_H
# define CLASSLIB_IO_ERROR_H
# include "classlib/utils/Error.h"

namespace lat {

/** I/O-operation related exception class. */
class IOError : public Error
{
public:
    IOError (const char *context, int code = 0);
    IOError (const char *context, const char *errmsg);
    IOError (const char *context, Error *chain);
    IOError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

protected:
    virtual std::string doexplain (const char *type) const;

private:
    std::string		m_context;
    std::string		m_errmsg;
};

} // namespace lat
#endif // CLASSLIB_IO_ERROR_H
