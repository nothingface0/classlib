#include "classlib/zip/TarInputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/zip/TarError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/inttypes.h"
#include <numeric>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>

namespace lat {

/** Initialise TAR archive reading from @a input, with @a format
    archive extensions supported. */
TarInputStream::TarInputStream (InputStream *input, Format format /* = ANY */)
    : FilterInputStream (input),
      m_current (0),
      m_remaining (0),
      m_padding (0),
      m_format (format),
      m_xfields (0),
      m_gxfields (0)
{}

/** Close the archive.  */
TarInputStream::~TarInputStream (void)
{ delete m_current; }

/** Return the number of bytes left to be read from the current
    archive member.

    This returns the number of bytes that should be left of the
    current member, it doesn't actually check whether there is
    anything available to be read from upstream.  */
IOOffset
TarInputStream::available (void)
{
    return m_current ? m_remaining : 0;
}

/** Read @a length bytes into @a buffer from the current archive
    member.  This can only be called if an archive member is
    currently open (see #nextMember()).  Returns zero if there is
    no more data to be read from the current archive member. */
IOSize
TarInputStream::read (void *into, IOSize n)
{
    // Must thave called nextMember() before reading data.
    if (! m_input || ! m_current)
	throw TarError (TarError::ARCHIVE_NOT_OPEN);

    // Refuse to read beyond end of current member
    if (! m_remaining)
	return 0;

    // We have a member to read from.  Don't read past its end.
    IOSize nread = m_input->read (into, std::min (n, IOSized (m_remaining)));
    m_remaining -= nread;

    // Report an error if archive member stops short.
    if (n && ! nread && m_remaining)
	throw TarError (TarError::DATA_SIZE_MISMATCH);

    return n;
}

/// Skip @a n bytes in the current archive member.
IOSize
TarInputStream::skip (IOSize n)
{
    // Must thave called nextMember() before reading data.
    if (! m_input || ! m_current)
	throw TarError (TarError::ARCHIVE_NOT_OPEN);

    // Skip data.
    IOSize skipped = m_input->skip (std::min (n, IOSized (m_remaining)));
    m_remaining -= skipped;

    // Report an error if archive member stops short.
    if (n && ! skipped && m_remaining)
	throw TarError (TarError::DATA_SIZE_MISMATCH);

    return skipped;
}

void
TarInputStream::close (void)
{
    delete m_current;
    m_current = 0;
    m_remaining = m_padding = 0;
    m_xfields = 0;
    m_gxfields = 0;
    m_xdata.reset();
    m_gxdata.reset();
    m_input->close ();
}

void
TarInputStream::reset (void)
{
    FilterInputStream::reset ();

    m_remaining = m_padding = 0;
    delete m_current;
    m_current = 0;

    m_xfields = 0;
    m_gxfields = 0;
    m_xdata.reset();
    m_gxdata.reset();
}

/** Read the header of the next archive member and get ready to read
    its data content.  */
TarMember *
TarInputStream::nextMember (void)
{
    // Must have input.
    if (! m_input)
	throw TarError (TarError::ARCHIVE_NOT_OPEN);

    // If a member is already open, finish reading it
    if (m_current)
	closeMember ();

    // Must be looking at the header and have closed previous one.
    ASSERT (m_input);
    ASSERT (! m_current);
    ASSERT (m_remaining == 0);
    ASSERT (m_padding == 0);

    // Read off the next header; we are in the right position to find
    // one here.  The end of archive is indicated by a block of zeroes
    // (block is 512 bytes = HEADER_SIZE), though it may not be there.
    // There can be trailing garbage after the end marker: the archive
    // has probably been rounded up to the record size (20 blocks by
    // default).
    // 
    // We don't get rid of the trailing junk: we don't know the record
    // size so we can't tell when it would end (FIXME?).  So once we
    // indicate stream end immediately upon seeing the marker.  Bad
    // news to anyone trying to read from the same stream after us...
    //
    // Note that we loop here to process all extended headers.
    m_xfields = 0;
    m_xdata.reset();
    unsigned char buf [HEADER_SIZE];
    IOSize n;

    while (true)
    {
        if (! extract (m_input, buf, HEADER_SIZE, HEADER_SIZE))
	    throw TarError (TarError::MISSING_HEADER);

        // Check if this is the end-of-archive marker of all zeroes.
	// Should not happen if we just read extended attribute data.
        for (n = 0; n < HEADER_SIZE && !buf [n]; ++n) ;
        if (n == HEADER_SIZE)
	{
	    if (m_xfields)
		throw TarError (TarError::UNSUPPORTED_FORMAT);
	    return 0;
	}

        // Validate the header. Do not validate version, it varies.
        // Check that all the numeric fields are ok and that string
        // fields are null terminated.
        if (strncmp ((const char *)buf+MAGIC_OFFSET, MAGIC, MAGIC_LENGTH)
	    || !validate (buf + MODE_OFFSET,     MODE_LENGTH)
	    || !validate (buf + UID_OFFSET,      UID_LENGTH)
	    || !validate (buf + GID_OFFSET,      GID_LENGTH)
	    || !validate (buf + SIZE_OFFSET,     SIZE_LENGTH)
	    || !validate (buf + MTIME_OFFSET,    MTIME_LENGTH)
	    || !validate (buf + CHKSUM_OFFSET,   CHKSUM_LENGTH)
	    || !validate (buf + DEVMAJOR_OFFSET, DEVMAJOR_LENGTH)
	    || !validate (buf + DEVMINOR_OFFSET, DEVMINOR_LENGTH)
	    || !memchr (buf + UNAME_OFFSET, 0, UNAME_LENGTH)
	    || !memchr (buf + GNAME_OFFSET, 0, GNAME_LENGTH))
	    throw TarError (TarError::HEADER_VALUE_ERROR);

        // Check the header checksum: sum of all 8-bit unsigned bytes in
        // the header, with checksum itself taken as blanks.  Some old
        // tars calculate the checksum from signed 8-bit bytes so accept
        // either.
        unsigned chksum = getOctal (buf + CHKSUM_OFFSET, CHKSUM_LENGTH-1);
        memset (buf + CHKSUM_OFFSET, CHKSUM_LENGTH, ' ');
        if (std::accumulate (buf, buf + HEADER_SIZE, 0u) != chksum
	    && std::accumulate ((signed char *) buf, (signed char *) buf + HEADER_SIZE, 0) != (int) chksum)
	    throw TarError (TarError::CHECKSUM_MISMATCH);

        // OK, it sure is a valid TAR header.  Extract the various values.
        uintmax_t mode     = getOctal (buf+MODE_OFFSET,     MODE_LENGTH-1);
        uintmax_t uid      = getOctal (buf+UID_OFFSET,      UID_LENGTH-1);
        uintmax_t gid      = getOctal (buf+GID_OFFSET,      GID_LENGTH-1);
        IOOffset  size     = getOctal (buf+SIZE_OFFSET,     SIZE_LENGTH-1);
        uintmax_t mtime    = getOctal (buf+MTIME_OFFSET,    MTIME_LENGTH-1);
        uintmax_t devmajor = getOctal (buf+DEVMAJOR_OFFSET, DEVMAJOR_LENGTH-1);
        uintmax_t devminor = getOctal (buf+DEVMINOR_OFFSET, DEVMINOR_LENGTH-1);
        unsigned char type = buf [TYPE_OFFSET];
    
        // Convert aliased type to canonical
        if (type == '\0')
	    type = REGULAR;

        // Check that the header is consistent.
        if (! (type == REGULAR
	       || type == HARD_LINK
	       || type == SYM_LINK
	       || type == CHAR_DEVICE
	       || type == BLOCK_DEVICE
	       || type == DIRECTORY
	       || type == FIFO
	       || type == CONTIGUOUS
	       || (type == EXTHDR && (m_format == ANY || m_format == PAX))
	       || (type == GLEXTHDR && (m_format == ANY || m_format == PAX)))
	       || (type == GNU_LONGLINK && (m_format == ANY || m_format == GNU))
	       || (type == GNU_LONGNAME && (m_format == ANY || m_format == GNU)))
	    throw TarError (TarError::UNSUPPORTED_FORMAT);

        // Unknown mode bits should be ignored.
        if (// Only links can have a link name
	       (buf [LINK_OFFSET] && ! (type == HARD_LINK || type == SYM_LINK))
	    // Links cannot have a size; ignored for directory, device, fifo and socket
	    || ((type == HARD_LINK || type == SYM_LINK) && size)
	    // Device numbers are only valid for devices
	    || ((devmajor || devminor) && ! (type == CHAR_DEVICE || type == BLOCK_DEVICE)))
	    throw TarError (TarError::PROPERTY_TOO_LARGE);

        // Now grab the various file names.
        std::string prefix (extractStr (buf + PREFIX_OFFSET, PREFIX_LENGTH));
        std::string name   (extractStr (buf + NAME_OFFSET,   NAME_LENGTH));
        std::string link   (extractStr (buf + LINK_OFFSET,   LINK_LENGTH));
	std::string uname  (extractStr (buf + UNAME_OFFSET,  UNAME_LENGTH));
	std::string gname  (extractStr (buf + GNAME_OFFSET,  GNAME_LENGTH));

        if (buf [PREFIX_OFFSET])
	    name = prefix + '/' + name;

	// If it is an extended header entry, stash the information away
	// rather than upating "m_current".
	if ((m_format == ANY || m_format == GNU) && type == GNU_LONGLINK)
	{
	    readExtendedData (link, size);
	    continue;
	}
	else if ((m_format == ANY || m_format == GNU) && type == GNU_LONGNAME)
	{
	    readExtendedData (name, size);
	    continue;
	}
	else if ((m_format == ANY || m_format == PAX) && type == EXTHDR)
	{
	    readExtendedAttrs (m_xdata, m_xfields, size);
	    continue;
	}
	else if ((m_format == ANY || m_format == PAX) && type == GLEXTHDR)
	{
	    // FIXME: Override option handling.
	    readExtendedAttrs (m_gxdata, m_gxfields, size);
	    continue;
	}

        // Create a new archive member (FIXME: override option handling).
        m_current = new TarMember;
        m_current->mode (mode);
        m_current->type (type);
        m_current->device (devmajor, devminor);

	if (m_xfields & XBIT_MTIME)
            m_current->mtime (m_xdata.mtime(), m_xdata.mtimefrac());
	else if (m_gxfields & XBIT_MTIME)
            m_current->mtime (m_gxdata.mtime(), m_gxdata.mtimefrac());
	else
            m_current->mtime (mtime);

	if (m_xfields & XBIT_ATIME)
            m_current->atime (m_xdata.atime(), m_xdata.atimefrac());
	else if (m_gxfields & XBIT_ATIME)
            m_current->atime (m_gxdata.atime(), m_gxdata.atimefrac());
	// no else, no atime in standard format

	if (m_xfields & XBIT_SIZE)
            m_current->size (m_xdata.size());
	else if (m_gxfields & XBIT_SIZE)
            m_current->size (m_gxdata.size());
	else
            m_current->size (size);

	if (m_xfields & XBIT_UID)
            m_current->uid (m_xdata.uid());
	else if (m_gxfields & XBIT_UID)
            m_current->uid (m_gxdata.uid());
	else
            m_current->uid (uid);

	if (m_xfields & XBIT_UNAME)
            m_current->uname (m_xdata.uname());
	else if (m_gxfields & XBIT_UNAME)
            m_current->uname (m_gxdata.uname());
	else
            m_current->uname (uname);

	if (m_xfields & XBIT_GID)
            m_current->gid (m_xdata.gid());
	else if (m_gxfields & XBIT_GID)
            m_current->gid (m_gxdata.gid());
	else
            m_current->gid (gid);

	if (m_xfields & XBIT_GNAME)
            m_current->gname (m_xdata.gname());
	else if (m_gxfields & XBIT_GNAME)
            m_current->gname (m_gxdata.gname());
	else
            m_current->gname (gname);

	if (m_xfields & XBIT_LINKPATH)
            m_current->linkname (m_xdata.linkname());
	else if (m_gxfields & XBIT_LINKPATH)
            m_current->linkname (m_gxdata.linkname());
	else
            m_current->linkname (link);

	if (m_xfields & XBIT_PATH)
            m_current->name (m_xdata.name());
	else if (m_gxfields & XBIT_PATH)
            m_current->name (m_gxdata.name());
	else
            m_current->name (name);

        // Calculate how many bytes must be read off after this file to
        // get back to the block boundary
	size = m_current->size();
        m_remaining = size;
        m_padding = (size + HEADER_SIZE-1) / HEADER_SIZE * HEADER_SIZE - size;
        ASSERT ((size + m_padding) % HEADER_SIZE == 0);

        // Return the new member.
        return m_current;
    }
}

/** Skip to the end of the current member and position the stream to
    be ready to read the next archive member.  */
void
TarInputStream::closeMember (void)
{
    // Skip any remaining data.
    m_remaining += m_padding;
    if (IOOffset (m_input->skip (IOSized (m_remaining))) != m_remaining)
	throw TarError (TarError::DATA_SIZE_MISMATCH);

    // Get rid of current member.
    m_remaining = m_padding = 0;
    delete m_current;
    m_current = 0;
    m_xfields = 0;
    m_xdata.reset();
}

/** Check whether @a length bytes from @a buf represent a valid octal
    number.  TAR format documentation says the fields are zero-filled
    ASCII octal integer terminated with a space and a null character.
    The reality is different.  We accept anything that is zero- or
    space-filled, has the right length, and has any number of trailing
    spaces and nulls.  */
bool
TarInputStream::validate (const void *buffer, IOSize length)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);

    // Allow leading spaces (should be zeroes, but often are not)
    for ( ; length && *buf == ' '; --length, ++buf)
	;

    // Now check for digits
    for (--length; length; --length, ++buf)
	if (*buf < '0' || *buf > '7')
	    break;

    // Remaining data can be nulls or spaces
    for (++length; length; --length, ++buf)
	if (*buf != ' ' && *buf != '\0')
	    return false;

    return true;
}

/** Extract a possibly null-terminated string starting at @a buffer and
    of maximum length @a max.  */
std::string
TarInputStream::extractStr (const void *buffer, IOSize max)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);
    const unsigned char *end = (const unsigned char *) memchr (buf, 0, max);
    return std::string ((const char *) buf, end ? end - buf : max);
}

/** Read extended data field @a size bytes long into @a data.
    Reads the data, handling rounding to archive block size. */
void
TarInputStream::readExtendedData (std::string &data, IOOffset size)
{
   // Make sure we can fit this in memory.
   if (IOOffset (IOSized (size)) != size)
	throw TarError (TarError::PROPERTY_TOO_LARGE);

   // Convert to suitable types and compute how much padding follows data.
   IOSize datalen = IOSized (size);
   IOSize total = (datalen + HEADER_SIZE-1) / HEADER_SIZE * HEADER_SIZE;

   // Read the data in, then skip the padding.
   data.resize (datalen, '\0');
   if (m_input->read (&data[0], datalen) != datalen
       || m_input->skip (total - datalen) != total - datalen)
	throw TarError (TarError::DATA_SIZE_MISMATCH);
}

/** Read PAX extended attributes field @a size bytes long into @a xdata.
    Reads the data record, handling rounding to archive block size,
    then parses the data to extended attribute values. */
void
TarInputStream::readExtendedAttrs (TarMember &xdata, uint8_t &xfields, IOOffset size)
{
    // Read the extended data in.
    std::string data;
    readExtendedData (data, size);

    // Parse the fields in format "%d %s=%s\n" @ (length, keyword, value).
    // The length gives the length of the entire record, including length
    // and the newline.  Note that value is read as binary data, and may
    // contain e.g. null bytes (technically it's supposed to be UTF8, but
    // we don't check for that).

    // Note that below the "p" is guaranteed to be null-terminated.
    intmax_t len;
    intmax_t reclen;
    const char *p = data.c_str();
    char *end;

    // Keep parsing records until we reach the end.
    for (len = data.size(); len; p += reclen, len -= reclen)
    {
	// Read next length part.  Bail out if it is too large.
	end = 0;
	errno = 0;
	reclen = strtoimax(p, &end, 10);
	if (errno || reclen <= 0 || reclen > len || ! end || end == p
	    || p[reclen] != '\n' || *end != ' ')
	   throw TarError (TarError::HEADER_VALUE_ERROR);

	// Find property name end at the first equal sign.
	const char *propname = ++end;
	if (! (end = strchr(propname, '=')) || end - p > reclen)
	    throw TarError (TarError::HEADER_VALUE_ERROR);
	ptrdiff_t proplen = end++ - propname;
	size_t attrlen =  p+reclen-1-end;

	// Process the extended attribute.  The name is now in
	// propname[0...proplen].  The attribute value is from
	// end to p+reclen-1.
	if (proplen == 5 && ! strncmp(propname, "atime", proplen))
	{
	    end = 0;
	    errno = 0;
	    double frac = 0;
	    uintmax_t val = strtoumax(end, &end, 10);
	    if (errno || ! end || end > p+reclen-1)
		throw TarError (TarError::HEADER_VALUE_ERROR);

	    if (*end == '.')
	    {
		end = 0;
		frac = strtod(end, &end);
		if (errno || ! end || end != p+reclen-1 || *end != '\n')
		    throw TarError (TarError::HEADER_VALUE_ERROR);
	    }
	    else if (end != p+reclen-1 || *end != '\n')
		throw TarError (TarError::HEADER_VALUE_ERROR);

	    xdata.atime (val, frac);
	    xfields |= XBIT_ATIME;
	}
	else if (proplen == 7
		 && (! strncmp(propname, "charset", 7)
		     || ! strncmp(propname, "comment", 7)))
	     /* Ignored */;
	else if (proplen == 3 && ! strncmp(propname, "gid", proplen))
	{
	    end = 0;
	    errno = 0;
	    uintmax_t val = strtoumax(end, &end, 10);
	    if (errno || ! end || end != p+reclen-1 || *end != '\n')
		throw TarError (TarError::HEADER_VALUE_ERROR);
	    xdata.gid (val);
	    xfields |= XBIT_GID;
	}
	else if (proplen == 5 && ! strncmp(propname, "gname", proplen))
	{
	    xdata.gname (std::string(end, attrlen));
	    xfields |= XBIT_GNAME;
	}
	else if (proplen == 8 && ! strncmp(propname, "linkpath", proplen))
	{
	    xdata.linkname (std::string(end, attrlen));
	    xfields |= XBIT_LINKPATH;
	}
	else if (proplen == 5 && ! strncmp(propname, "mtime", proplen))
	{
	    end = 0;
	    errno = 0;
	    double frac = 0;
	    uintmax_t val = strtoumax(end, &end, 10);
	    if (errno || ! end || end > p+reclen-1)
		throw TarError (TarError::HEADER_VALUE_ERROR);

	    if (*end == '.')
	    {
		end = 0;
		frac = strtod(end, &end);
		if (errno || ! end || end != p+reclen-1 || *end != '\n')
		    throw TarError (TarError::HEADER_VALUE_ERROR);
	    }
	    else if (end != p+reclen-1 || *end != '\n')
		throw TarError (TarError::HEADER_VALUE_ERROR);

	    xdata.atime (val, frac);
	    xfields |= XBIT_ATIME;
	}
	else if (proplen == 4 && ! strncmp(propname, "path", proplen))
	{
	    xdata.name (std::string(end, attrlen));
	    xfields |= XBIT_PATH;
	}
	else if (proplen == 4 && ! strncmp(propname, "size", proplen))
	{
	    end = 0;
	    errno = 0;
	    uintmax_t val = strtoumax(end, &end, 10);
	    if (errno || ! end || end != p+reclen-1 || *end != '\n')
		throw TarError (TarError::HEADER_VALUE_ERROR);
	    xdata.size (val);
	    xfields |= XBIT_SIZE;
	}
	else if (proplen == 3 && ! strncmp(propname, "uid", proplen))
	{
	    end = 0;
	    errno = 0;
	    uintmax_t val = strtoumax(end, &end, 10);
	    if (errno || ! end || end != p+reclen-1 || *end != '\n')
		throw TarError (TarError::HEADER_VALUE_ERROR);
	    xdata.uid (val);
	    xfields |= XBIT_UID;
	}
	else if (proplen == 5 && ! strncmp(propname, "uname", proplen))
	{
	    xdata.uname (std::string(end, attrlen));
	    xfields |= XBIT_UNAME;
	}
	// else ignored
    }
}

} // namespace lat
