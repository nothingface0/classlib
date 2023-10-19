#include "classlib/zip/Checksum.h"

namespace lat {

/** Destroy a checksum.  No-op.  */
Checksum::~Checksum (void)
{}

/** Update the checksum by one @a byte.  Redirects the call to
    #update(const void *, IOSize).  */
void
Checksum::update (unsigned char byte)
{ update (&byte, 1); }

/** Update the checksum by @a data.  Redirects the call to
    #update(const void *, IOSize).  */
void
Checksum::update (IOBuffer data)
{ update (data.data (), data.size ()); }

} // namespace lat
