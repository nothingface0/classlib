#include "classlib/zip/CPIOInputStream.h"
#include "classlib/zip/CPIOMember.h"
#include "classlib/zip/CPIOError.h"
#include "classlib/utils/DebugAids.h"
#include <numeric>
#include <string>
#include <cstring>

namespace lat {

/** Initialise CPIO archive reading from @a input.  If @a check and
    the archive is SVR4 archive with a checksum (#SVR4C_MAGIC), also
    validate the simple checksum while the data is read in. */
CPIOInputStream::CPIOInputStream (InputStream *input, bool check /* = false */)
    : FilterInputStream (input),
      m_format (0),
      m_current (0),
      m_remaining (0),
      m_checksum (0),
      m_sum (0),
      m_check (check)
{}

/** Close the archive.  */
CPIOInputStream::~CPIOInputStream (void)
{ delete m_current; }

/** Return the number of bytes left to be read from the current
    archive member.

    This returns the number of bytes that should be left of the
    current member, it doesn't actually check whether there is
    anything available to be read from upstream.  */
IOOffset
CPIOInputStream::available (void)
{
    return m_current ? m_remaining : 0;
}

/** Read @a length bytes into @a buffer from the current archive
    member.  This can only be called if an archive member is
    currently open (see #nextMember()).  Returns zero if there is
    no more data to be read from the current archive member. */
IOSize
CPIOInputStream::read (void *into, IOSize n)
{
    // Must have called nextMember() before reading data.
    if (! m_input || ! m_current)
	throw CPIOError (CPIOError::ARCHIVE_NOT_OPEN);

    // Refuse to read beyond end of current member
    if (! m_remaining)
	return 0;

    // We have a member to read from.  Don't read past its end.
    IOSize nread = m_input->read (into, std::min (n, IOSized (m_remaining)));
    if (m_check && m_format == SVR4C_MAGIC)
        m_sum = std::accumulate ((unsigned char *) into,
			         (unsigned char *) into + nread,
			         m_sum);
    m_remaining -= nread;

    // Report an error if archive member stops short.
    if (n && ! nread && m_remaining)
	throw CPIOError (CPIOError::DATA_SIZE_MISMATCH);

    return nread;
}

/// Skip @a n bytes in the current archive member.
IOSize
CPIOInputStream::skip (IOSize n)
{
    // Must have called nextMember() before reading data.
    if (! m_input || ! m_current)
	throw CPIOError (CPIOError::ARCHIVE_NOT_OPEN);

    // Skip data.  If we are validating checksum, we have to read.
    IOSize skipped = 0;
    if (m_check && m_format == SVR4C_MAGIC)
    {
	IOSize did = 1;
	unsigned char buf[4096];
	while (skipped < n && did)
	{
	    did = m_input->read (buf, std::min (std::min<IOSize> (sizeof(buf), n),
				    		IOSized (m_remaining)));
	    m_sum = std::accumulate (buf, buf + did, m_sum);
	    m_remaining -= did;
	    skipped += did;
	}
    }
    else if (n > 0)
    {
        skipped = m_input->skip (std::min (n, IOSized (m_remaining)));
        m_remaining -= skipped;
    }

    // Report an error if archive member stops short.
    if (n && ! skipped && m_remaining)
	throw CPIOError (CPIOError::DATA_SIZE_MISMATCH);

    return skipped;
}

void
CPIOInputStream::close (void)
{
    delete m_current;
    m_current = 0;
    m_remaining = 0;
    m_checksum = m_sum = 0;
    m_input->close ();
}

void
CPIOInputStream::reset (void)
{
    FilterInputStream::reset ();

    m_remaining = 0;
    m_checksum = m_sum = 0;
    delete m_current;
    m_current = 0;
}

/** Read the header of the next archive member and get ready to read
    its data content.  Returns null when the end of archive has been
    reached: a special "trailer" entry is seen. */
CPIOMember *
CPIOInputStream::nextMember (void)
{
    // Must have input.
    if (! m_input)
	throw CPIOError (CPIOError::ARCHIVE_NOT_OPEN);

    // If a member is already open, finish reading it
    if (m_current)
	closeMember ();

    // Must be looking at the header and have closed previous one.
    ASSERT (! m_current);
    ASSERT (m_remaining == 0);
    ASSERT (m_checksum == 0);
    ASSERT (m_sum == 0);

    // Read off the next header; we are in the right position to find
    // one here.  The end of archive is indicated by a member whose
    // name is CPIOConstants::TRAILER (= "TRAILER!!!").  We automatically
    // distinguish between various archive formats.
    unsigned char buf [MAGIC_LEN];
    if (! extract (m_input, buf, sizeof (buf), sizeof (buf)))
	throw CPIOError (CPIOError::MISSING_HEADER);

    // Validate the header.  This depends on what type of archive
    // we have.  The old POSIX.1 archive has all octal numerics,
    // SVR4 has the fields in hex.  The only difference between
    // SVR4 and SVR4-with-checksum is that the latter has a checksum
    // of all the file data (it's not a CRC!).
    switch (m_format = getOctal (buf, MAGIC_LEN))
    {
    case ODC_MAGIC:
	return readPosixHeader ();

    case SVR4_MAGIC:
    case SVR4C_MAGIC:
	return readSvrHeader ();

    default:
	throw CPIOError (CPIOError::UNSUPPORTED_FORMAT);
    }
}

/** Read POSIX.1 CPIO header.  */
CPIOMember *
CPIOInputStream::readPosixHeader (void)
{
    unsigned		ino;
    unsigned		mode;
    unsigned		uid;
    unsigned		gid;
    unsigned		nlink;
    unsigned		rdev;
    unsigned		dev;
    unsigned		mtime;
    unsigned		namelen;
    IOOffset		size;
    std::string		name;
    unsigned char	buf [ODC_HEADER_LEN];

    if (! extract (m_input, buf+MAGIC_LEN,
		   sizeof (buf)-MAGIC_LEN,
		   sizeof (buf)-MAGIC_LEN))
	throw CPIOError (CPIOError::MISSING_HEADER);

    if (!checkOctal (buf + ODC_DEV_OFFSET,     ODC_DEV_LENGTH)
	|| !checkOctal (buf + ODC_INO_OFFSET,     ODC_INO_LENGTH)
	|| !checkOctal (buf + ODC_MODE_OFFSET,    ODC_MODE_LENGTH)
	|| !checkOctal (buf + ODC_UID_OFFSET,     ODC_UID_LENGTH)
	|| !checkOctal (buf + ODC_GID_OFFSET,     ODC_GID_LENGTH)
	|| !checkOctal (buf + ODC_NLINK_OFFSET,   ODC_NLINK_LENGTH)
	|| !checkOctal (buf + ODC_RDEV_OFFSET,    ODC_RDEV_LENGTH)
	|| !checkOctal (buf + ODC_MTIME_OFFSET,   ODC_MTIME_LENGTH)
	|| !checkOctal (buf + ODC_NAMELEN_OFFSET, ODC_NAMELEN_LENGTH)
	|| !checkOctal (buf + ODC_SIZE_OFFSET,    ODC_SIZE_LENGTH))
	throw CPIOError (CPIOError::HEADER_VALUE_ERROR);

    // It's valid POSIX header, extract the values.
    dev     = getOctal (buf + ODC_DEV_OFFSET,     ODC_DEV_LENGTH);
    ino     = getOctal (buf + ODC_INO_OFFSET,     ODC_INO_LENGTH);
    mode    = getOctal (buf + ODC_MODE_OFFSET,    ODC_MODE_LENGTH);
    uid     = getOctal (buf + ODC_UID_OFFSET,     ODC_UID_LENGTH);
    gid     = getOctal (buf + ODC_GID_OFFSET,     ODC_GID_LENGTH);
    nlink   = getOctal (buf + ODC_NLINK_OFFSET,   ODC_NLINK_LENGTH);
    rdev    = getOctal (buf + ODC_RDEV_OFFSET,    ODC_RDEV_LENGTH);
    mtime   = getOctal (buf + ODC_MTIME_OFFSET,   ODC_MTIME_LENGTH);
    namelen = getOctal (buf + ODC_NAMELEN_OFFSET, ODC_NAMELEN_LENGTH);
    size    = getOctal (buf + ODC_SIZE_OFFSET,    ODC_SIZE_LENGTH);

    // Check that the header is consistent (FIXME: check various modes vs size!)
    // and get the file name.
    name.resize (namelen, '\0');
    if (namelen == 0
	|| ! extract (m_input, &name [0], namelen, namelen)
	|| name [namelen-1] != '\0')
	throw CPIOError (CPIOError::MISSING_HEADER);
    name.erase (namelen-1, 1);

    // Check for the magic end-of-archive marker.
    if (name == TRAILER)
	return 0;

    // Create a new archive member (FIXME: symlink handling?)
    m_current = new CPIOMember (name);
    m_current->dev (dev);
    m_current->ino (ino);
    m_current->mode (mode);
    m_current->uid (uid);
    m_current->gid (gid);
    m_current->nlink (nlink);
    m_current->rdev (rdev);
    m_current->mtime (mtime);
    m_current->size (size);

    // Remember how large this file was.
    m_remaining = size;

    // Return the new member.
    return m_current;
}

/** Read SVR4 CPIO header; if @a check, validate checksum too.  */
CPIOMember *
CPIOInputStream::readSvrHeader (void)
{
    unsigned		ino;
    unsigned		mode;
    unsigned		uid;
    unsigned		gid;
    unsigned		nlink;
    unsigned		rdevmajor;
    unsigned		rdevminor;
    unsigned		devmajor;
    unsigned		devminor;
    unsigned		mtime;
    unsigned		namelen;
    IOOffset		size;
    unsigned		checksum;
    std::string		name;
    unsigned char	buf [SVR_HEADER_LEN];

    if (! extract (m_input, buf+MAGIC_LEN,
		   sizeof (buf)-MAGIC_LEN,
		   sizeof (buf)-MAGIC_LEN))
	throw CPIOError (CPIOError::MISSING_HEADER);

    if (!checkHex (buf + SVR_INO_OFFSET,       SVR_INO_LENGTH)
	|| !checkHex (buf + SVR_MODE_OFFSET,      SVR_MODE_LENGTH)
	|| !checkHex (buf + SVR_UID_OFFSET,       SVR_UID_LENGTH)
	|| !checkHex (buf + SVR_GID_OFFSET,       SVR_GID_LENGTH)
	|| !checkHex (buf + SVR_NLINK_OFFSET,     SVR_NLINK_LENGTH)
	|| !checkHex (buf + SVR_MTIME_OFFSET,     SVR_MTIME_LENGTH)
	|| !checkHex (buf + SVR_SIZE_OFFSET,      SVR_SIZE_LENGTH)
	|| !checkHex (buf + SVR_DEVMAJOR_OFFSET,  SVR_DEVMAJOR_LENGTH)
	|| !checkHex (buf + SVR_DEVMINOR_OFFSET,  SVR_DEVMINOR_LENGTH)
	|| !checkHex (buf + SVR_RDEVMAJOR_OFFSET, SVR_RDEVMAJOR_LENGTH)
	|| !checkHex (buf + SVR_RDEVMINOR_OFFSET, SVR_RDEVMINOR_LENGTH)
	|| !checkHex (buf + SVR_NAMELEN_OFFSET,   SVR_NAMELEN_LENGTH)
	|| !checkHex (buf + SVR_CHECKSUM_OFFSET,  SVR_CHECKSUM_LENGTH))
	throw CPIOError (CPIOError::HEADER_VALUE_ERROR);

    // It's valid SVR4 header, extract the values.
    ino       = getHex (buf + SVR_INO_OFFSET,       SVR_INO_LENGTH);
    mode      = getHex (buf + SVR_MODE_OFFSET,      SVR_MODE_LENGTH);
    uid       = getHex (buf + SVR_UID_OFFSET,       SVR_UID_LENGTH);
    gid       = getHex (buf + SVR_GID_OFFSET,       SVR_GID_LENGTH);
    nlink     = getHex (buf + SVR_NLINK_OFFSET,     SVR_NLINK_LENGTH);
    mtime     = getHex (buf + SVR_MTIME_OFFSET,     SVR_MTIME_LENGTH);
    size      = getHex (buf + SVR_SIZE_OFFSET,      SVR_SIZE_LENGTH);
    devmajor  = getHex (buf + SVR_DEVMAJOR_OFFSET,  SVR_DEVMAJOR_LENGTH);
    devminor  = getHex (buf + SVR_DEVMINOR_OFFSET,  SVR_DEVMINOR_LENGTH);
    rdevmajor = getHex (buf + SVR_RDEVMAJOR_OFFSET, SVR_RDEVMAJOR_LENGTH);
    rdevminor = getHex (buf + SVR_RDEVMINOR_OFFSET, SVR_RDEVMINOR_LENGTH);
    namelen   = getHex (buf + SVR_NAMELEN_OFFSET,   SVR_NAMELEN_LENGTH);
    checksum  = getHex (buf + SVR_CHECKSUM_OFFSET,  SVR_CHECKSUM_LENGTH);

    // Check that the header is consistent.
    name.resize (namelen, '\0');
    if (namelen == 0
	|| ! extract (m_input, &name [0], namelen, namelen)
	|| name [namelen-1] != '\0')
	throw CPIOError (CPIOError::MISSING_HEADER);
    name.erase (namelen-1, 1);

    // File contents and header are padded to four in SVR archives
    // and two in binary archives; POSIX archive isn't padded at all.
    char paddata [4];
    unsigned padding = 4 - ((SVR_HEADER_LEN + namelen) % 4);
    if (! extract (m_input, paddata, padding, padding))
	throw CPIOError (CPIOError::MISSING_PADDING);

    // Check for the magic end-of-archive marker.
    if (name == TRAILER)
	return 0;

    // Create a new archive member (FIXME: symlink handling?)
    m_current = new CPIOMember (name);
    m_current->dev (devmajor, devminor);
    m_current->ino (ino);
    m_current->mode (mode);
    m_current->uid (uid);
    m_current->gid (gid);
    m_current->nlink (nlink);
    m_current->rdev (rdevmajor, rdevminor);
    m_current->mtime (mtime);
    m_current->size (size);

    // Remember how large this file was.
    m_remaining = size;

    // Remember we have checksum.
    m_checksum = checksum;
    m_sum = 0;

    // Return the new member.
    return m_current;
}

/** Skip to the end of the current member and position the stream to
    be ready to read the next archive member.  */
void
CPIOInputStream::closeMember (void)
{
    if (IOOffset (m_input->skip (IOSized (m_remaining))) != m_remaining)
	throw CPIOError (CPIOError::DATA_SIZE_MISMATCH);

    // File contents is padded to four in SVR archives and two in
    // binary archives; POSIX archive isn't padded at all.
    if (m_format == SVR4_MAGIC || m_format == SVR4C_MAGIC)
    {
        char paddata [4];
        unsigned padding = 4 - (m_current->size () % 4);
        if (! extract (m_input, paddata, padding, padding))
	    throw CPIOError (CPIOError::MISSING_PADDING);
    }

    // Check checksum if so requested (SVR4C_MAGIC).
    if (m_check && m_format == SVR4C_MAGIC && m_checksum != m_sum)
	throw CPIOError (CPIOError::CHECKSUM_MISMATCH);

    // Get rid of the current member.
    m_remaining = 0;
    m_checksum = m_sum = 0;
    delete m_current;
    m_current = 0;
}

/** Check whether @a length bytes from @a buffer represent a valid hex
    number.  CPIO format documentation says the fields are zero-filled
    ASCII hex integers with no field separators.  */
bool
CPIOInputStream::checkHex (const void *buffer, IOSize length)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);
    for (--length ; length; --length, ++buf)
	if (! isxdigit (*buf))
	    return false;

    return true;
}

/** Check whether @a length bytes from @a buffer represent a valid octal
    number.  CPIO format documentation says the fields are zero-filled
    ASCII octal integers with no field separators.  */
bool
CPIOInputStream::checkOctal (const void *buffer, IOSize length)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);
    for (--length ; length; --length, ++buf)
	if (*buf < '0' || *buf > '7')
	    return false;

    return true;
}

} // namespace lat
