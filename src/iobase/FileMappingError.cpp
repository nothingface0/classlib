
#include "classlib/iobase/FileMappingError.h"

namespace lat {

FileMappingError::FileMappingError (const char *context, int code /* = 0 */)
    : IOError (context, code)
{}

FileMappingError::FileMappingError (const char *context, Error *chain)
    : IOError (context, chain)
{}

FileMappingError::FileMappingError (Error *chain)
    : IOError (chain)
{}

std::string
FileMappingError::explainSelf (void) const
{ return IOError::doexplain ("Memory mapping operation"); }

Error *
FileMappingError::clone (void) const
{ return new FileMappingError (*this); }

void
FileMappingError::rethrow (void)
{ throw *this; }

} // namespace lat
