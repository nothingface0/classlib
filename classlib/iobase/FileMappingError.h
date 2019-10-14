#ifndef CLASSLIB_FILE_MAPPING_ERROR_H
# define CLASSLIB_FILE_MAPPING_ERROR_H
# include "classlib/iobase/IOError.h"

namespace lat {

/** File-system related exception class. */
class FileMappingError : public IOError
{
public:
    FileMappingError (const char *context, int code = 0);
    FileMappingError (const char *context, Error *chain);
    FileMappingError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

} // namespace lat
#endif // CLASSLIB_FILE_MAPPING_ERROR_H
