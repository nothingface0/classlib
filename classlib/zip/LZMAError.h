#ifndef CLASSLIB_LZMA_ERROR_H
# define CLASSLIB_LZMA_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** LZO error indication.  */
class LZMAError : public Error
{
public:
    LZMAError (const char *context, int code);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    virtual int		code (void) const;
    virtual const char *context (void) const;
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    const char		*m_context;	//< LZMA error context
    int			m_code;		//< LZMA error code
};

} // namespace lat
#endif // CLASSLIB_LZMA_ERROR_H
