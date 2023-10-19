#include "classlib/zip/TarOutputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/zip/TarError.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/utils/UserInfo.h"
#include "classlib/sysapi/inttypes.h"
#include <numeric>
#include <string>
#include <cstring>
#include <cstdio>

namespace lat {

TarOutputStream::TarOutputStream (OutputStream *output,
				  bool pad /* = true */,
				  Format format /* = PAX */)
    : FilterOutputStream (output),
      m_current (0),
      m_buffer (0),
      m_remaining (0),
      m_padding (0),
      m_blocks (0),
      m_pad (pad),
      m_format (format),
      m_finished (false)
{
    if (format != USTAR && format != PAX && format != GNU)
	throw TarError (TarError::UNSUPPORTED_FORMAT);
}

/** Close the archive.  */
TarOutputStream::~TarOutputStream (void)
{
    delete m_current;
    delete m_buffer;
}

/** Define whether trailing archive padding should be written.

    If @a pad is @c true, the archive will be padded to the default
    record size (20 blocks of 512 bytes) with null bytes, as per the
    TAR archive format specification.  If @a pad is @c false, only
    the end-of-archive marker is written but no padding.  The padding
    can be omitted if the client knows that the archive is not being
    written to a media requiring the padding (= a tape).  By default
    the padding is on (see the constructor).

    FIXME: Take record size/blocking factor instead?  */
void
TarOutputStream::setPadding (bool pad)
{
    m_pad = pad;
}

/** Write the header of the next archive member and get ready to write
    its data content.

    Either @a member's size must equal to the amount of data that will
    be written afterwards with write(), or @a autobuffer must be set;
    it is not possible to write any of the member data before its
    total size is known.  If @a autobuffer is off, we simply record
    the size given here and at the end of the member assert that an
    equal amount of data was actually written.  If @a autobuffer is
    on, nothing is written until the member is finished; at that time
    the header and entire contents will be written out.  Writing large
    files with autobuffering is very inefficient -- if you do know the
    size beforehand, do not buffer the contents.

    This class takes ownership of @a member.  */
void
TarOutputStream::nextMember (TarMember *member, bool autobuffer /* = false */)
{
    // Can't be at the end already.
    if (! m_output || m_finished)
	throw TarError (TarError::ARCHIVE_NOT_OPEN);

    // Close previous member if it is still open.
    if (m_current)
	closeMember ();

    // Must be at the end of previous member.
    ASSERT (! m_current);
    ASSERT (! m_buffer);
    ASSERT (m_remaining == 0);
    ASSERT (m_padding == 0);

    // Must have new member.
    ASSERT (member);
    m_current = member;

    // If automatic buffering is requested, postpone everything until
    // we see the end of data stream.  Otherwise write out the header;
    // we are at the right position and know everything we need.
    if (autobuffer)
	m_buffer = new MemoryStorage;
    else
	writeHeader ();
}

/** Dump out an archive member header for the current member.  */
void
TarOutputStream::writeHeader (void)
{
    unsigned char buf [HEADER_SIZE];
    memset (buf, 0, sizeof (buf));

    // Get the member values
    std::string		xattrs;
    std::string		prefix;
    std::string		name	 (m_current->name ());
    std::string		linkname (m_current->linkname ());
    std::string		uname	 = m_current->uname ();
    std::string		gname	 = m_current->gname ();
    uintmax_t		mode	 = m_current->mode ();
    uintmax_t		uid	 = m_current->uid ();
    uintmax_t		gid	 = m_current->gid ();
    intmax_t		size	 = m_current->size ();
    intmax_t		mtime	 = m_current->mtime ();
    double		mtimefr  = m_current->mtimefrac ();
    intmax_t		atime    = m_current->atime ();
    double		atimefr  = m_current->atimefrac ();
    uintmax_t		devmajor = m_current->devmajor ();
    uintmax_t		devminor = m_current->devminor ();
    unsigned char	type	 = m_current->type ();

    // Check basic field correctness before doing any more work.
    // We allow callers to pass thru extended data manually, even
    // if that capability shouldn't be particularly useful.
    if (mtime < 0 || mtimefr < 0 || atime < 0 || atimefr < 0
        || mode > SMALL_MAX || devmajor > SMALL_MAX || devminor > SMALL_MAX
	|| (type != REGULAR && type != HARD_LINK && type != SYM_LINK
	    && type != CHAR_DEVICE && type != BLOCK_DEVICE
	    && type != DIRECTORY && type != FIFO && type != CONTIGUOUS
	    && type != EXTHDR && type != GLEXTHDR
	    && type != GNU_LONGLINK && type != GNU_LONGNAME))
	throw TarError (TarError::PROPERTY_TOO_LARGE);

    // Link name is only valid for a link.
    if (! linkname.empty () && type != HARD_LINK && type != SYM_LINK)
	throw TarError (TarError::HEADER_VALUE_ERROR);
    
    // Size must be zero for links and directories.
    if ((type == HARD_LINK || type == SYM_LINK
	 || type == DIRECTORY || type == FIFO
	 || type == CHAR_DEVICE || type == BLOCK_DEVICE)
	&& size)
	throw TarError (TarError::HEADER_VALUE_ERROR);

    // Device major, minor are only valid for device types.
    if ((devmajor || devminor) && type != CHAR_DEVICE && type != BLOCK_DEVICE)
	throw TarError (TarError::HEADER_VALUE_ERROR);

    // Split the file name into prefix and name parts.  If it is
    // longer than what will fit into name, try to split off a part
    // into prefix, at a slash boundary.  From the end of the file
    // name we look for a slash which would leave `name' no longer
    // than NAME_LENGTH and `prefix' no longer than PREFIX_LENGTH.
    // This may not succeed; if it fails, we raise a format exception
    // to signal that the file cannot be put in a TAR archive.
    if (name.length () > NAME_LENGTH)
    {
	const char *begin = name.c_str ();
	const char *end   = begin + name.length ();
	const char *p     = end;
	do
	    for (--p ; p != begin && *p != '/'; --p) ;
	while (p != begin
	       && p - begin > PREFIX_LENGTH
	       && end - p <= NAME_LENGTH);

	ASSERT (p >= begin);
	ASSERT (p < end);

	if (p - begin <= PREFIX_LENGTH && end - p <= NAME_LENGTH)
	{
	    ASSERT (*p == '/');
	    prefix = std::string (begin, p);
	    name   = std::string (p+1, end);
	}
	else if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" path=%1\n").arg (name));
	    name.resize (NAME_LENGTH);
	}
	else if (m_format == GNU)
	{
	    writeExtraData (GNU_LONGNAME, name, buf);
	    name.resize (NAME_LENGTH);
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If link target is too long, create extended header or bail out.
    if (linkname.size() > LINK_LENGTH)
    {
	if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" linkpath=%1\n").arg (linkname));
	    linkname.resize (LINK_LENGTH);
	}
	else if (m_format == GNU)
	{
	    writeExtraData (GNU_LONGLINK, name, buf);
	    linkname.resize (LINK_LENGTH);
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If uid is too big, create extended header or bail out.
    if (uid > SMALL_MAX)
    {
	if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" uid=%1\n").arg (uid));
	    uid = SMALL_MAX;
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If uname is too long, create extended header or bail out.
    if (uname.size() > UNAME_LENGTH)
    {
	if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" uname=%1\n").arg (uname));
	    uname.resize (UNAME_LENGTH);
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If gid is too big, create extended header or bail out.
    if (gid > SMALL_MAX)
    {
	if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" gid=%1\n").arg (gid));
	    gid = SMALL_MAX;
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If gname is too long, create extended header or bail out.
    if (gname.size() > GNAME_LENGTH)
    {
	if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" gname=%1\n").arg (gname));
	    gname.resize (GNAME_LENGTH);
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If size is too big, create extended header or bail out.
    if (size > LARGE_MAX)
    {
	if (m_format == PAX)
	{
	    xattrs += encodeAttr (StringFormat (" size=%1\n").arg (size));
	    size = LARGE_MAX;
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }
    else if (size < 0)
	throw TarError (TarError::PROPERTY_TOO_LARGE);

    // If mtime is too big or we have fractional part, create extended
    // header or bail out.
    if (mtime > LARGE_MAX || mtimefr != 0)
    {
	if (m_format == PAX)
	{
	    if (mtimefr != 0)
	        xattrs += encodeAttr (StringFormat (" mtime=%1%2\n")
				      .arg (mtime).arg(mtimefr, 0, 'f', 9));
	    else
	        xattrs += encodeAttr (StringFormat (" mtime=%1\n").arg (mtime));

	    mtime = LARGE_MAX;
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If we have a atime, create extended header or bail out.
    if (atime || atimefr != 0)
    {
	if (m_format == PAX)
	{
	    if (atimefr != 0)
	        xattrs += encodeAttr (StringFormat (" atime=%1%2\n")
				      .arg (atime).arg(atimefr, 0, 'f', 9));
	    else
	        xattrs += encodeAttr (StringFormat (" atime=%1\n").arg (atime));
	}
	else
	    throw TarError (TarError::PROPERTY_TOO_LARGE);
    }

    // If we accumulated extra attributes, write them out now.
    if (xattrs.size ())
	writeExtraData (EXTHDR, xattrs, buf);

    // Put in the field values.  Note that we don't use the official
    // documented version string, but do what GNU tar does.
    putOctal (buf +	MODE_OFFSET,     MODE_LENGTH,     mode);
    putOctal (buf +	UID_OFFSET,      UID_LENGTH,      uid);
    putOctal (buf +	GID_OFFSET,      GID_LENGTH,      gid);
    putOctal (buf +	SIZE_OFFSET,     SIZE_LENGTH,     IOSized (size));
    putOctal (buf +	MTIME_OFFSET,    MTIME_LENGTH,    mtime);
    if (type == CHAR_DEVICE || type == BLOCK_DEVICE)
    {
	putOctal (buf +	DEVMAJOR_OFFSET, DEVMAJOR_LENGTH, devmajor);
	putOctal (buf +	DEVMINOR_OFFSET, DEVMINOR_LENGTH, devminor);
    }

    strncpy  ((char *)buf + MAGIC_OFFSET,   MAGIC,             MAGIC_LENGTH);
    strncpy  ((char *)buf + NAME_OFFSET,    name.c_str (),     NAME_LENGTH);
    strncpy  ((char *)buf + PREFIX_OFFSET,  prefix.c_str (),   PREFIX_LENGTH);
    strncpy  ((char *)buf + LINK_OFFSET,    linkname.c_str (), LINK_LENGTH);
    strncpy  ((char *)buf + UNAME_OFFSET,   uname.c_str (),    UNAME_LENGTH);
    strncpy  ((char *)buf + GNAME_OFFSET,   gname.c_str (),    GNAME_LENGTH);
    buf [VERSION_OFFSET] = ' ';
    buf [VERSION_OFFSET+1] = '\0';
    buf [TYPE_OFFSET] = type;

    // Compute checksum and put it in
    memset (buf + CHKSUM_OFFSET, ' ', CHKSUM_LENGTH);
    unsigned checksum = std::accumulate (buf, buf + HEADER_SIZE, 0u);
    ASSERT (checksum <= SMALL_MAX);
    putOctal (buf + CHKSUM_OFFSET, CHKSUM_LENGTH, checksum);

    // Write out the header and update block count
    m_output->xwrite (buf, sizeof (buf));
    m_blocks++;

    // Calculate how much padding to are needed at the end (from original size)
    size = m_current->size();
    m_padding = (size + HEADER_SIZE-1) / HEADER_SIZE * HEADER_SIZE - size;
    m_remaining = size;

    // Update block count now, otherwise we'd have to keep track of
    // partial blocks written in write().  So just count how many
    // blocks it will take to write out m_remaining + m_padding bytes.
    ASSERT (m_padding < HEADER_SIZE);
    ASSERT ((m_remaining + m_padding) % HEADER_SIZE == 0);
    m_blocks += (m_remaining + m_padding) / HEADER_SIZE;
}

/** Write @a length bytes from @a buffer into the current archive
    member.  */
IOSize
TarOutputStream::write (const void *from, IOSize n)
{
    // Can't be at the end already and must have an open member.
    if (! m_output || m_finished || ! m_current)
	throw TarError (TarError::ARCHIVE_NOT_OPEN);

    // If auto-buffering stash the output away
    if (m_buffer)
    {
	ASSERT (! m_remaining);
	ASSERT (! m_padding);
	return m_buffer->write (from, n);
    }

    // Can't ovewrite advertised file size.
    else if (m_remaining < IOOffset (n))
	throw TarError (TarError::DATA_SIZE_MISMATCH);

    // OK, write through.
    else
    {
	IOSize did = m_output->xwrite (from, n);
	m_remaining -= did;
	return did;
    }
}

/** Close the current member and get ready to write the next one or to
    close the stream terminate.  */
void
TarOutputStream::closeMember (void)
{
    // Can't be at the end already and must have an open member.
    if (! m_output || m_finished || ! m_current)
	throw TarError (TarError::ARCHIVE_NOT_OPEN);

    // Must have written out all advertised data.
    if (m_remaining)
	throw TarError (TarError::DATA_SIZE_MISMATCH);

    // If auto-buffering, dump the whole thing now with header and
    // all gathered data, then zap the buffer.  Verify advertised
    // size was zero or matches the amount of buffered data.
    if (m_buffer)
    {
	intmax_t bufsize = m_buffer->data().size();
	if (m_current->size() && m_current->size() != bufsize)
	    throw TarError (TarError::DATA_SIZE_MISMATCH);

	m_current->size (bufsize);
	writeHeader ();
	m_output->xwrite (m_buffer->data ());
	m_remaining = 0;
	delete m_buffer;
	m_buffer = 0;
    }

    // Write out trailing padding to get us back to block boundary
    if (m_padding)
    {
        ASSERT (m_padding < HEADER_SIZE);
	unsigned char buf [HEADER_SIZE];
	memset (buf, 0, sizeof (buf));
	m_output->xwrite (buf, IOSize (m_padding));
	m_padding = 0;
    }

    // Get rid of the current member.
    delete m_current;
    m_current = 0;
}

/** Finish writing to the stream by padding it up to the default TAR
    record size (20 blocks).  (FIXME: only one null block if the
    output goes to a file?)  */
void
TarOutputStream::finish (void)
{
    if (! m_output || m_finished)
	return;

    // Finish off the current member if one is still open.
    if (m_current)
	closeMember ();

    // Cowardly refuse to write an empty archive.
    if (! m_blocks)
	throw TarError (TarError::EMPTY_ARCHIVE);

    ASSERT (! m_current);
    ASSERT (! m_buffer);

    // Write the end-of-archive marker
    unsigned char buf [HEADER_SIZE];
    memset (buf, 0, sizeof (buf));
    m_output->xwrite (buf, sizeof (buf));
    m_blocks++;

    // Pad to the default record size (20 blocks).  The client can turn this
    // off, e.g. if it is known that the output goes to the file and the
    // trailing padding is useless.  This makes the archive nominally
    // non-conforming, but saves space and other tar programs do it anyway.
    for ( ; m_pad && m_blocks % 20; ++m_blocks)
	m_output->xwrite (buf, sizeof (buf));

    // Close context.
    m_remaining = m_padding = 0;
    m_output->finish ();
    m_finished = true;
}

void
TarOutputStream::writeExtraData (Type type,
				 const std::string &data,
				 unsigned char *buf)
{
    // Get the member values
    uintmax_t	uid	 = UserInfo::self()->uid();
    uintmax_t	gid	 = UserInfo::self()->gid();
    intmax_t	size	 = data.size();
    intmax_t	mtime	 = Time::current().ns() / TimeConst::SEC_NSECS;

    if (uid > SMALL_MAX)
	uid = SMALL_MAX;

    if (gid > SMALL_MAX)
	gid = SMALL_MAX;

    if (mtime > LARGE_MAX)
	mtime = LARGE_MAX;

    if (size > LARGE_MAX)
	throw TarError (TarError::FILE_TOO_LARGE);

    // Put in the field values.  Note that we don't use the official
    // documented version string, but do what GNU tar does.
    memset(buf, 0, HEADER_SIZE);
    putOctal (buf +	MODE_OFFSET,      MODE_LENGTH,   0644);
    putOctal (buf +	UID_OFFSET,       UID_LENGTH,    uid);
    putOctal (buf +	GID_OFFSET,       GID_LENGTH,    gid);
    putOctal (buf +	SIZE_OFFSET,      SIZE_LENGTH,   size);
    putOctal (buf +	MTIME_OFFSET,     MTIME_LENGTH,  mtime);
    strncpy  ((char *)buf + MAGIC_OFFSET, MAGIC,         MAGIC_LENGTH);
    strncpy  ((char *)buf + NAME_OFFSET,  "././@xattrs", NAME_LENGTH);
    buf [VERSION_OFFSET] = ' ';
    buf [VERSION_OFFSET+1] = '\0';
    buf [TYPE_OFFSET] = type;

    // Compute checksum and put it in
    memset (buf + CHKSUM_OFFSET, ' ', CHKSUM_LENGTH);
    unsigned checksum = std::accumulate (buf, buf + HEADER_SIZE, 0u);
    ASSERT (checksum <= SMALL_MAX);
    putOctal (buf + CHKSUM_OFFSET, CHKSUM_LENGTH, checksum);

    // Write out the header and update block count
    m_output->xwrite (buf, sizeof (buf));
    m_blocks++;

    // Write the data itself.
    m_output->xwrite (data.c_str(), data.size());

    // Pad this entry.
    memset (buf, 0, HEADER_SIZE);
    intmax_t pad = (size + HEADER_SIZE-1) / HEADER_SIZE * HEADER_SIZE - size;
    ASSERT (pad >= 0 && pad < HEADER_SIZE);
    if (pad)
	m_output->xwrite (buf, IOSize (pad));

    // Update block count.
    ASSERT ((size + pad) % HEADER_SIZE == 0);
    m_blocks += (size + pad) / HEADER_SIZE;
}

#if __GNUC__
# pragma GCC diagnostic ignored "-Wformat" // shut warning on '%zu'
#endif

/** Encode extended attribute key/value pair @a keyval into the format
    defined in the PAX standard: "%d %s=%s\n", where the leading number
    is the length of the entire record including the length itself and
    the trailing newline.  This routine assumes the @a keyval pair
    already includes the leading space and trailing newline, and just
    adds the length prefix in front.  Returns the final full string. */
std::string
TarOutputStream::encodeAttr (const std::string &keyval)
{
    char nbuf[BitTraits<size_t>::Digits+1];
    size_t len = keyval.size();
    size_t n = 0;
    size_t p;

    do
    {
	p = n;
	n = sprintf(nbuf, __extension__ "%zu", len + p);
    } while (n != p);

    return nbuf + keyval;
}

} // namespace lat
