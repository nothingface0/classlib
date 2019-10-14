#ifndef CLASSLIB_REGEXP_ERROR_H
# define CLASSLIB_REGEXP_ERROR_H
# include "classlib/utils/Error.h"
# include <string>

namespace lat {

/** File-system related exception class. */
class RegexpError : public Error
{
public:
    RegexpError (const char *context, int syntax, int error);
    RegexpError (const char *context, int syntax, const char *msg);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    const char	*m_context;
    int		m_syntax;
    int		m_error;
    std::string	m_message;
};

} // namespace lat
#endif // CLASSLIB_REGEXP_ERROR_H
