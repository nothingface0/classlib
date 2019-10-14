#include "classlib/zip/LZMAError.h"
#include "classlib/utils/BitTraits.h"
#include <lzma.h>
#include <string>
#include <cstdio>
#include <cstring>

namespace lat {

/** Construct a new error object from the LZMA error @a code. */
LZMAError::LZMAError (const char *context, int code)
    : m_context (context),
      m_code (code)
{}

/** Return the LZMA error code: why this exception was thrown.  */
int
LZMAError::code (void) const
{ return m_code; }

/** Return the LZMA error context: where this exception occurred.  */
const char *
LZMAError::context (void) const
{ return m_context; }

/** Return a string describing this error.  FIXME: I18N? */
std::string
LZMAError::explainSelf (void) const
{
#define UNKNOWN_MESSAGE	"LZMA unknown error (%d)"
    char	buf [sizeof(UNKNOWN_MESSAGE)-2 + BitTraits<int>::Digits];
    const char	*msg;

    switch (m_code)
    {
    case LZMA_OK:
	msg = "LZMA success (no error)";
	break;

    case LZMA_NO_CHECK:
	msg = "LZMA input stream has no integrity check";
	break;

    case LZMA_UNSUPPORTED_CHECK:
	msg = "LZMA cannot calculate integrity check";
	break;

    case LZMA_GET_CHECK:
	msg = "LZMA integrity check is now available";
	break;

    case LZMA_MEM_ERROR:
	msg = "LZMA cannot allocate memory";
	break;

    case LZMA_MEMLIMIT_ERROR:
	msg = "LZMA memory usage limit was reached";
	break;

    case LZMA_FORMAT_ERROR:
	msg = "LZMA file format not recognised";
	break;

    case LZMA_OPTIONS_ERROR:
	msg = "LZMA invalid or unsupported options";
	break;

    case LZMA_DATA_ERROR:
	msg = "LZMA data is corrupt";
	break;

    case LZMA_BUF_ERROR:
	msg = "LZMA no progress is possible";
	break;

    case LZMA_PROG_ERROR:
	msg = "LZMA programming error";
	break;

    // Something we don't know about
    default:
	sprintf (buf, UNKNOWN_MESSAGE, m_code);
	msg = buf;
	break;
    }

    std::string result;
    result.reserve (strlen (msg) + 4 + strlen (m_context));
    result += msg;
    result += " in ";
    result += m_context;
    return result;
}

/** @reimp */
Error *
LZMAError::clone (void) const
{ return new LZMAError (*this); }

/** @reimp */
void
LZMAError::rethrow (void)
{ throw *this; }

} // namespace lat
