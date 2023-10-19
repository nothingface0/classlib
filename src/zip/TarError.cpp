#include "classlib/zip/TarError.h"
#include "classlib/utils/DebugAids.h"
#include <string>

namespace lat {

/** Construct a new TAR error object. */
TarError::TarError (Reason reason)
    : m_reason (reason)
{}

/** Return the reason code describing this error. */
TarError::Reason
TarError::reason (void) const
{ return m_reason; }

/** Return a string describing this error. */
std::string
TarError::explainSelf (void) const
{
    switch (m_reason)
    {
    case ARCHIVE_NOT_OPEN:
        return "TAR: No open archive member.";

    case MISSING_HEADER:
        return "TAR: Missing archive member header.";

    case DATA_SIZE_MISMATCH:
        return "TAR: Unexpected end of archive data.";

    case CHECKSUM_MISMATCH:
        return "TAR: Checksum mismatch.";

    case UNSUPPORTED_FORMAT:
        return "TAR: Unsupported archive format.";

    case HEADER_VALUE_ERROR:
        return "TAR: Invalid field value in archive header.";

    case FILE_TOO_LARGE:
        return "TAR: File too large for archive format.";

    case PROPERTY_TOO_LARGE:
        return "TAR: File property value to big for archive format.";

    case EMPTY_ARCHIVE:
        return "TAR: Cowardly refusing to write an empty archive.";

    default:
        ASSERT(false);
        return "TAR: Unknown error.";
    }
}

/** @reimp */
Error *
TarError::clone (void) const
{ return new TarError (*this); }

/** @reimp */
void
TarError::rethrow (void)
{ throw *this; }

} // namespace lat
