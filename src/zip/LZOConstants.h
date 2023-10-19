#ifndef CLASSLIB_LZO_CONSTANTS_H
# define CLASSLIB_LZO_CONSTANTS_H

namespace lat {

/** Constants related to LZO compression.  */
struct LZOConstants
{
    /// LZO compression algorithm family.
    enum Algorithm
    {
	LZO1,	//< LZO1
	LZO1A,	//< LZO1A
	LZO1B,	//< LZO1B
	LZO1C,	//< LZO1C
	LZO1F,	//< LZO1F
	LZO1X,	//< LZO1X
	LZO1Y,	//< LZO1Y
	LZO1Z,	//< LZO1Z
	LZO2A	//< LZO2A
    };
};

} // namespace lat
#endif // CLASSLIB_LZO_CONSTANTS_H
