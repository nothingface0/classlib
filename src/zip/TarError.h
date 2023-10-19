#ifndef CLASSLIB_TAR_ERROR_H
# define CLASSLIB_TAR_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** Tar archive error indication.  */
class TarError : public Error
{
public:
    enum Reason
    {
	/** No currently open archive member. */
	ARCHIVE_NOT_OPEN,

	/** An archive member header was expected but none was found. */
	MISSING_HEADER,

	/** Unexpected end of archive member data, or mismatch in
	    written and advertised data when writing. */
	DATA_SIZE_MISMATCH,

	/** Archive header checksum error. */
	CHECKSUM_MISMATCH,

	/** Unsupported TAR archive member type. */
	UNSUPPORTED_FORMAT,

	/** Archive member header field failed validation of valid values. */
	HEADER_VALUE_ERROR,

	/** File is too large for this format, given supported extensions. */
	FILE_TOO_LARGE,

	/** File property other than size is too large for this format,
	    given selected support for extensions, or field was set but
	    not expected to be set for a particular member type. */
	PROPERTY_TOO_LARGE,

	/** Attempted to create an empty archive. */
	EMPTY_ARCHIVE
    };

    TarError (Reason reason);
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
#endif // CLASSLIB_TAR_ERROR_H
