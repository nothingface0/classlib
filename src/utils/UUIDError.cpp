
#include "classlib/utils/UUIDError.h"

namespace lat {

UUIDError::UUIDError (void)
    : Error (0)
{}

std::string
UUIDError::explainSelf (void) const
{ return "UUID Format Error"; }

Error *
UUIDError::clone (void) const
{ return new UUIDError (*this); }

void
UUIDError::rethrow (void)
{ throw *this; }

} // namespace lat
