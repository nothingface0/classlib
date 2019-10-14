#include "classlib/zip/LZOError.h"
#include "classlib/utils/BitTraits.h"
#include <lzo/lzoconf.h>
#include <string>
#include <cstdio>
#include <cstring>

namespace lat {

/** Construct a new error object from the LZO error @a code. */
LZOError::LZOError (const char *context, int code)
    : m_context (context),
      m_code (code)
{}

/** Return the LZO error code: why this exception was thrown.  */
int
LZOError::code (void) const
{ return m_code; }

/** Return the LZO error context: where this exception occurred.  */
const char *
LZOError::context (void) const
{ return m_context; }

/** Return a string describing this error.  FIXME: I18N? */
std::string
LZOError::explainSelf (void) const
{
#define UNKNOWN_MESSAGE	"LZO unknown error (%d)"
    char	buf [sizeof(UNKNOWN_MESSAGE)-2 + BitTraits<int>::Digits];
    const char	*msg;

    switch (m_code)
    {
    case LZO_E_OK:
	msg = "LZO success (no error)";
	break;

    case LZO_E_ERROR:
	msg = "LZO unspecified error";
	break;

    case LZO_E_OUT_OF_MEMORY:
	msg = "LZO out of memory";
	break;

    case LZO_E_NOT_COMPRESSIBLE:
	msg = "LZO data not compressible";
	break;

    case LZO_E_INPUT_OVERRUN:
	msg = "LZO input overrun";
	break;

    case LZO_E_OUTPUT_OVERRUN:
	msg = "LZO output overrun";
	break;

    case LZO_E_LOOKBEHIND_OVERRUN:
	msg = "LZO lookbehind overrun";
	break;

    case LZO_E_EOF_NOT_FOUND:
	msg = "LZO EOF not found";
	break;

    case LZO_E_INPUT_NOT_CONSUMED:
	msg = "LZO input not consumed";
	break;

    case LZO_E_NOT_YET_IMPLEMENTED:
	msg = "LZO not yet implemented";
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
LZOError::clone (void) const
{ return new LZOError (*this); }

/** @reimp */
void
LZOError::rethrow (void)
{ throw *this; }

} // namespace lat
