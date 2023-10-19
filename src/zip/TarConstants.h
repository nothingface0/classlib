#ifndef CLASSLIB_TAR_CONSTANTS_H
# define CLASSLIB_TAR_CONSTANTS_H
# include "classlib/zip/ZipUtils.h"

namespace lat {

/** Constants related to TAR archives.

    There is one fairly well defined archive format standardised by
    POSIX (1003.1-1996) called USTAR.  It is slightly better than CPIO
    format in that it permits user and group information to be preserved
    by name.  However storing long file names and large files takes some
    acrobatics.

    There is TAR-compatible extended format PAX standardised by POSIX.
    This does permit storage of almost all the information regardless
    of limits, but not all systems have PAX-compatible tools.  This
    library supports PAX in limited ways.  In particular character
    encodings are not supported; all data is assumed to be UTF8.

    See http://www.opengroup.org/onlinepubs/009695399/utilities/pax.html
    for the POSIX format definitions (USTAR and PAX).  */
struct TarConstants : ZipUtils
{
    // Known formats
    enum Format
    {
	ANY,		//< Use or accept any known format.
	USTAR,		//< POSIX USTAR standard, no extensions.
	PAX,		//< POSIX PAX standard extensions.
	GNU		//< GNU TAR extensions.
    };

    // Unix file mode bits
    enum Mode
    {
	SUID		= 04000,//< Set-UID bit
	SGID		= 02000,//< Set-GID bit
	SVTX		= 01000,//< Sticky text bit
	UREAD		= 00400,//< User read bit
	UWRITE		= 00200,//< User write bit
	UEXEC		= 00100,//< User execute bit
	GREAD		= 00040,//< Group read bit
	GWRITE		= 00020,//< Group write bit
	GEXEC		= 00010,//< Group execute bit
	OREAD		= 00004,//< Other read bit
	OWRITE		= 00002,//< Other write bit
	OEXEC		= 00001 //< Other execute bit
    };

    // Archive member type
    enum Type
    {
	REGULAR		= '0',	//< Regular file
	HARD_LINK	= '1',	//< Hard link to another archive member
	SYM_LINK	= '2',	//< Symbolic link: no data, only link
	CHAR_DEVICE	= '3',	//< Character device: no data, only dev numbers
	BLOCK_DEVICE	= '4',	//< Block device: no data, only dev numbers
	DIRECTORY	= '5',	//< Directory: no data
	FIFO		= '6',	//< FIFO: no data, only name
	CONTIGUOUS	= '7',	//< Contiguously stored (regular) file
	EXTHDR          = 'x',	//< PAX extended header data for the next archive member
	GLEXTHDR        = 'g',	//< PAX extended header data for all subsequent members
	GNU_LONGLINK    = 'K',	//< Next member in archive has long link name
	GNU_LONGNAME    = 'L'	//< Next member in archive has long name

	// Known but unsupported:
	//  SOLARIS_ACL  = 'A'  //< Solaris access control list
	//  SOLARIS_EA   = 'E'  //< Solaris extended attribute file
	//  SOLARIS_EXT  = 'X'  //< Solaris extended header
	//  STAR_INODE   = 'I'  //< Inode only ('star')
	//  VU_EXTENDED  = 'X'  //< POSIX 1003.1-2001 extended (VU version)
	//  GNU_DIR_DUMP = 'D'	//< Directory entry with the file names at
	// 			//  the time the dump was made
	//  GNU_MULTIVOL = 'M'	//< Continuation of a file on another volume
	//  GNU_NAMES    = 'N'	//< Filenames that did not fit in the main hdr
	//  GNU_SPARSE   = 'S'	//< Sparse file
	//  GNU_VOLHDR   = 'V'	//< Tape/volume header; ignore on extraction
    };

    // Header magic and version number strings
    static const char	*MAGIC;        //< TAR archive magic (POSIX 1003.1-90)
    static const char	*VERSION;      //< Header version identification string
    static const char   *PAX_ATIME;    //< PAX atime extended header.
    static const char   *PAX_CHARSET;  //< PAX charset extended header.
    static const char   *PAX_COMMENT;  //< PAX comment extended header.
    static const char   *PAX_GID;      //< PAX gid extended header.
    static const char   *PAX_GNAME;    //< PAX group name extended header.
    static const char   *PAX_LINKPATH; //< PAX link name extended header.
    static const char   *PAX_MTIME;    //< PAX mtime extended header.
    static const char   *PAX_PATH;     //< PAX path extended header.
    static const char   *PAX_SIZE;     //< PAX size extended header.
    static const char   *PAX_UID;      //< PAX uid extended header.
    static const char   *PAX_UNAME;    //< PAX user name extended header.

    // Offsets into the central directory headers
    enum
    {
	NAME_OFFSET	= 0,	NAME_LENGTH	= 100,
	MODE_OFFSET	= 100,	MODE_LENGTH	= 8,
	UID_OFFSET	= 108,	UID_LENGTH	= 8,
	GID_OFFSET	= 116,	GID_LENGTH	= 8,
	SIZE_OFFSET	= 124,	SIZE_LENGTH	= 12,
	MTIME_OFFSET	= 136,	MTIME_LENGTH	= 12,
	CHKSUM_OFFSET	= 148,	CHKSUM_LENGTH	= 8,
	TYPE_OFFSET	= 156,	TYPE_LENGTH	= 1,
	LINK_OFFSET	= 157,	LINK_LENGTH	= 100,
	MAGIC_OFFSET	= 257,	MAGIC_LENGTH	= 6,
	VERSION_OFFSET	= 263,	VERSION_LENGTH	= 2,
	UNAME_OFFSET	= 265,	UNAME_LENGTH	= 32,
	GNAME_OFFSET	= 297,	GNAME_LENGTH	= 32,
	DEVMAJOR_OFFSET	= 329,	DEVMAJOR_LENGTH	= 8,
	DEVMINOR_OFFSET	= 329,	DEVMINOR_LENGTH	= 8,
	PREFIX_OFFSET	= 345,	PREFIX_LENGTH	= 155,

	HEADER_SIZE	= 512,

	// Maximum values for numeric values
	SMALL_MAX	= 07777777,	 // 7 octal digits (or spaces), null: uid...
	LARGE_MAX	= 07777777777    // 11 octal digits, space: size, mtime
    };

    // Bits to mark which extended attributes have been set.
    enum
    {
	XBIT_ATIME    = 1 << 0,		//< Have PAX_ATIME.
	XBIT_GID      = 1 << 1,		//< Have PAX_GID.
	XBIT_GNAME    = 1 << 2,		//< Have PAX_GNAME.
	XBIT_LINKPATH = 1 << 3,		//< Have PAX_LINKPATH or GNU_LONGLINK.
	XBIT_MTIME    = 1 << 4,		//< Have PAX_MTIME.
	XBIT_PATH     = 1 << 5,		//< Have PAX_PATH or GNU_LONGNAME.
	XBIT_SIZE     = 1 << 6,		//< Have PAX_SIZE.
	XBIT_UID      = 1 << 7,		//< Have PAX_UID.
	XBIT_UNAME    = 1 << 8		//< Have PAX_UNAME.
    };
};

} // namespace lat
#endif // CLASSLIB_TAR_CONSTANTS_H
