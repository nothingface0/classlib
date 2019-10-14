#ifndef CLASSLIB_CPIO_CONSTANTS_H
# define CLASSLIB_CPIO_CONSTANTS_H
# include "classlib/zip/ZipUtils.h"

namespace lat {

/** Constants related to CPIO archives.

    CPIO archive formats are not very standard.  They are also
    deprecated because of the inherent limits in file size and
    large UID/GID values, and inability to represent users and
    groups as names, rather than just numerical values.
    
    Single UNIX Specfication (SUSv2) and POSIX (1003.1-1996) define
    an ASCII format known as the "Portable ASCII Format" or "Old
    Character Format" (ODC); this format supports files up to 8GB in
    size and 262143 in file name length, UID, GID and device ID.
    Hard linked files copy file data for every file instance.
    
    SVR4 defines "new" ASCII format, with and without checksum, known
    as "ASC" or "-c" format, and "CRC" format, respectively.  The
    format supports files only up to 4GB in size, but does allow
    larger values (2^32-1) for the other fields and separate major,
    minor device ID numbers.  The checksum is only a simple linear
    32-bit unsigned sum of the file contents, not a CRC.  Hard linked
    files set file size to zero for all but the last instance of the
    file in the archive.

    There is also a binary format which is platform-specific, byte
    order included.  Most of the fields are limited to 65535 (2^16-1),
    but file size is limited to 2GB.  The binary format fields are
    documented below, but the format is not otherwise supported by
    this library.

    Note that the device and inode values may be synthesized.  They
    should be identical for hard-linked file, otherwise any unique
    numbering will do.
    
    See http://www.opengroup.org/onlinepubs/009695399/utilities/pax.html
    for the POSIX format definitions (ODC, USTAR and PAX).  See also
    http://people.freebsd.org/~kientzle/libarchive/man/cpio.5.txt for
    additional documentation on the different CPIO formats.  */
struct CPIOConstants : ZipUtils
{
    /** Name of the magic last member of the archive. */
    static const char *TRAILER;

    enum
    {
	ODC_MAGIC	= 070707,	//< POSIX ODC format.
	SVR4_MAGIC	= 070701,	//< SVR4 archive ("ASC" or -c)
	SVR4C_MAGIC	= 070702	//< SVR4 archive with checksum ("CRC")
	// BIN_MAGIC	= 070707	//< Binary archive (unsupported)
	// BBS_MAGIC	= 0143561	//< Binary byte swap archive (unsupported)
    };

    // Unix file mode bits
    enum Mode
    {
	SSOCK		= 0140000,	//< Socket
	SLNK		= 0120000,	//< Symbolic link
	SCTG		= 0110000,	//< Reserved
	SREG		= 0100000,	//< Regular file
	SBLK		= 0060000,	//< Block special file
	SDIR		= 0040000,	//< Directory
	SCHR		= 0020000,	//< Character special file
	SFIFO		= 0010000,	//< FIFO
	SUID		= 0004000,	//< Set-UID bit
	SGID		= 0002000,	//< Set-GID bit
	SVTX		= 0001000,	//< Sticky text bit
	UREAD		= 0000400,	//< User read bit
	UWRITE		= 0000200,	//< User write bit
	UEXEC		= 0000100,	//< User execute bit
	GREAD		= 0000040,	//< Group read bit
	GWRITE		= 0000020,	//< Group write bit
	GEXEC		= 0000010,	//< Group execute bit
	OREAD		= 0000004,	//< Other read bit
	OWRITE		= 0000002,	//< Other write bit
	OEXEC		= 0000001	//< Other execute bit
    };

    // Offsets into the archive header: ascii and binary
    enum
    {
	MAGIC_LEN		= 6,
	TRAILER_LEN		= 11,

	ODC_MAGIC_OFFSET	= 0,	ODC_MAGIC_LENGTH	= 6,
	ODC_DEV_OFFSET		= 6,	ODC_DEV_LENGTH		= 6,
	ODC_INO_OFFSET		= 12,	ODC_INO_LENGTH		= 6,
	ODC_MODE_OFFSET		= 18,	ODC_MODE_LENGTH		= 6,
	ODC_UID_OFFSET		= 24,	ODC_UID_LENGTH		= 6,
	ODC_GID_OFFSET		= 30,	ODC_GID_LENGTH		= 6,
	ODC_NLINK_OFFSET	= 36,	ODC_NLINK_LENGTH	= 6,
	ODC_RDEV_OFFSET		= 42,	ODC_RDEV_LENGTH		= 6,
	ODC_MTIME_OFFSET	= 48,	ODC_MTIME_LENGTH	= 11,
	ODC_NAMELEN_OFFSET	= 59,	ODC_NAMELEN_LENGTH	= 6, // Includes trailing null
	ODC_SIZE_OFFSET		= 65,	ODC_SIZE_LENGTH		= 11,
	ODC_HEADER_LEN		= 76,

	SVR_MAGIC_OFFSET	= 0,	SVR_MAGIC_LENGTH	= 6,
	SVR_INO_OFFSET		= 6,	SVR_INO_LENGTH		= 8,
	SVR_MODE_OFFSET		= 14,	SVR_MODE_LENGTH		= 8,
	SVR_UID_OFFSET		= 22,	SVR_UID_LENGTH		= 8,
	SVR_GID_OFFSET		= 30,	SVR_GID_LENGTH		= 8,
	SVR_NLINK_OFFSET	= 38,	SVR_NLINK_LENGTH	= 8,
	SVR_MTIME_OFFSET	= 46,	SVR_MTIME_LENGTH	= 8,
	SVR_SIZE_OFFSET		= 54,	SVR_SIZE_LENGTH		= 8,
	SVR_DEVMAJOR_OFFSET	= 62,	SVR_DEVMAJOR_LENGTH	= 8,
	SVR_DEVMINOR_OFFSET	= 70,	SVR_DEVMINOR_LENGTH	= 8,
	SVR_RDEVMAJOR_OFFSET	= 78,	SVR_RDEVMAJOR_LENGTH	= 8,
	SVR_RDEVMINOR_OFFSET	= 86,	SVR_RDEVMINOR_LENGTH	= 8,
	SVR_NAMELEN_OFFSET	= 94,	SVR_NAMELEN_LENGTH	= 8, // Padded to mod4
	SVR_CHECKSUM_OFFSET	= 102,	SVR_CHECKSUM_LENGTH	= 8,
	SVR_HEADER_LEN		= 110,
	SVR_TRAILER_PAD		= 3,

	BIN_MAGIC_OFFSET	= 0,	BIN_MAGIC_LENGTH	= 2,  // short
	BIN_DEV_OFFSET		= 2,	BIN_DEV_LENGTH		= 2,  // short
	BIN_INO_OFFSET		= 4,	BIN_INO_LENGTH		= 2,  // ushort
	BIN_MODE_OFFSET		= 6,	BIN_MODE_LENGTH		= 2,  // ushort
	BIN_UID_OFFSET		= 8,	BIN_UID_LENGTH		= 2,  // ushort
	BIN_GID_OFFSET		= 10,	BIN_GID_LENGTH		= 2,  // ushort
	BIN_NLINK_OFFSET	= 12,	BIN_NLINK_LENGTH	= 2,  // short
	BIN_RDEV_OFFSET		= 14,	BIN_RDEV_LENGTH		= 2,  // short
	BIN_MTIME_OFFSET	= 16,	BIN_MTIME_LENGTH	= 4,  // short[2]
	BIN_NAMELEN_OFFSET	= 20,	BIN_NAMELEN_LENGTH	= 2,  // short
	BIN_FILELEN_OFFSET	= 22,	BIN_FILELEN_LENGTH	= 4,  // short[2]
	BIN_HEADER_LEN		= 26,

	// Maximum values for numeric values
	ODC_SMALL_MAX		= 0777777,	// 262143 (2^18-1)
	SVR_FIELD_MAX		= 0xffffffff	// 4294967295 (2^32-1)
    };

    static const IOOffset ODC_LARGE_MAX = LONG_LONG_CONST(077777777777); // 8589934592 (2^33-1)
};

} // namespace lat
#endif // CLASSLIB_CPIO_CONSTANTS_H
