#ifndef CLASSLIB_CPIO_ERROR_H
# define CLASSLIB_CPIO_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** Error indicator for CPIO archives.  */
class CPIOError : public Error
{
public:
    enum Reason
    {
	/** No currently open archive member. */
	ARCHIVE_NOT_OPEN,

	/** An archive member header was expected but none was found,
	    or not enough data could be read for a header. */
	MISSING_HEADER,

	/** An archive member was expected to be padded (SVR4 format). */
	MISSING_PADDING,

	/** Unexpected end of archive member data, or mismatch in
	    written and advertised data when writing. */
	DATA_SIZE_MISMATCH,

	/** Checksum error (#SVR4C_MAGIC). */
	CHECKSUM_MISMATCH,

	/** Unsupported CPIO archive style, not a CPIO archive at all. */
	UNSUPPORTED_FORMAT,

	/** Archive member header field failed validation of valid values. */
	HEADER_VALUE_ERROR,

	/** File is too large for this format (see #CPIOConstants). */
	FILE_TOO_LARGE,

	/** File property other than size is too large for this format.
	    See #CPIOConstants for the limits of the different fields. */
	PROPERTY_TOO_LARGE
    };

    CPIOError (Reason reason);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    Reason		reason (void) const;
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    Reason		m_reason;
};

} // namespace lat
#endif // CLASSLIB_CPIO_ERROR_H
