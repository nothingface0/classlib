#include "classlib/zip/CPIOError.h"
#include "classlib/utils/DebugAids.h"
#include <string>

namespace lat {

/** Construct a new CPIO error object. */
CPIOError::CPIOError (Reason reason)
    : m_reason (reason)
{}

/** Return the reason code describing this error. */
CPIOError::Reason
CPIOError::reason (void) const
{ return m_reason; }

/** Return a string describing this error.  */
std::string
CPIOError::explainSelf (void) const
{
    switch (m_reason)
    {
    case ARCHIVE_NOT_OPEN:
	return "CPIO: No open archive member.";

    case MISSING_HEADER:
	return "CPIO: Missing archive member header.";

    case MISSING_PADDING:
	return "CPIO: Missing archive padding.";

    case DATA_SIZE_MISMATCH:
	return "CPIO: Unexpected end of archive data.";

    case CHECKSUM_MISMATCH:
	return "CPIO: Checksum mismatch.";

    case UNSUPPORTED_FORMAT:
	return "CPIO: Unsupported archive format.";

    case HEADER_VALUE_ERROR:
	return "CPIO: Invalid field value in archive header.";

    case FILE_TOO_LARGE:
	return "CPIO: File too large for archive format.";

    case PROPERTY_TOO_LARGE:
	return "CPIO: File property value to big for archive format.";

    default:
	ASSERT(false);
	return "CPIO: Unknown error.";
    }
}

/** @reimp  */
Error *
CPIOError::clone (void) const
{ return new CPIOError (*this); }

/** @reimp  */
void
CPIOError::rethrow (void)
{ throw *this; }

} // namespace lat
