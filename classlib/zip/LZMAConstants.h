#ifndef CLASSLIB_LZMA_CONSTANTS_H
# define CLASSLIB_LZMA_CONSTANTS_H
# include "classlib/zip/ZipUtils.h"

namespace lat {

/** Constants related to LZMA compression.  */
struct LZMAConstants : ZipUtils
{
    // Compression levels
    enum { BEST_COMPRESSION		= 9 }; //< Best compression level
    enum { DEFAULT_COMPRESSION		= 6 }; //< Default compression level
    enum { BEST_SPEED			= 0 }; //< Fastest compression level

    enum Mode
    {
        ModeNormal,
	ModeFast
    };

    enum MatchFinder
    {
        MFHC3,
        MFHC4,
	MFBT2,
	MFBT3,
	MFBT4
    };

    enum Arch
    {
        ArchX86,
	ArchPowerPC,
	ArchIA64,
	ArchARM,
	ArchARMThumb,
	ArchSPARC
    };

    static const size_t      DefaultDictionarySize;
    static const MatchFinder DefaultMatchFinder;
    static const unsigned    DefaultLiteralContextBits;
    static const unsigned    DefaultLiteralPositionBits;
    static const unsigned    DefaultPositionBits;
    static const unsigned    DefaultNiceMatchLength;
    static const unsigned    DefaultMaxSearchDepth;
};

} // namespace lat
#endif // CLASSLIB_LZMA_CONSTANTS_H
