
#include "classlib/iotools/IOException.h"

namespace lat {

/** Construct an error object.  Simply constructs the chain.  */
IOException::IOException (Error *next /* = 0 */)
    : Error (next)
{}

/** Provide an explanation of this error.  */
std::string
IOException::explainSelf (void) const
{ return "I/O Error"; } 

/** @reimp  */
Error *
IOException::clone (void) const
{ return new IOException (*this); }

/** @reimp  */
void
IOException::rethrow (void)
{ throw *this; }

} // namespace lat
