#include "test/Test.h" 
#include "classlib/zip/LZMACompressor.h"
#include "classlib/zip/LZMADecompressor.h"
#include "classlib/zip/MD5Digest.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Signal.h"
#include <vector>

using namespace lat;
struct LZMACompressCheck
{
    unsigned level;
    bool extreme;
    IOSize csize;
    const char *checksum;
};

const LZMACompressCheck tests[] = {
    { 0, false, 131046, "1d4ea420aca1c1c09f3da0153e44d428" },
    { 1, false, 127824, "60efba79181a4ecc2072ac51d68077c4" },
    { 2, false, 120073, "0f85f39e91dc14bc2a379ba099f8f8e3" },
    { 3, false, 108734, "89f746b94bd30798b5460d6dea7e108c" },
    { 4, false, 108734, "89f746b94bd30798b5460d6dea7e108c" },
    { 5, false, 108734, "89f746b94bd30798b5460d6dea7e108c" },
    { 6, false, 108708, "f8d8217bd041cc3783a674cf0b05913e" },
    { 7, false, 108708, "f8d8217bd041cc3783a674cf0b05913e" },
    { 8, false, 108708, "f8d8217bd041cc3783a674cf0b05913e" },
    { 9, false, 108708, "f8d8217bd041cc3783a674cf0b05913e" },
    { 0, true , 113464, "e6e570b556a2966c4ddfaff8626c5aa9" },
    { 1, true , 113464, "e6e570b556a2966c4ddfaff8626c5aa9" },
    { 2, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 3, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 4, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 5, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 6, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 7, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 8, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" },
    { 9, true , 108921, "ce0fb24543fbf0c416415dded0b520a0" }
};

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const int    NUM = sizeof(tests) / sizeof(tests[0]);
    Filename            testfile (Filename (__FILE__).directory(), "test.txt");
    FileMapping         fmap (new File (testfile));
    IOBuffer            data = fmap.map ();
    std::vector<char>	comp (data.size() * 2, '\0');
    std::vector<char>	decomp (data.size() * 2, '\0');
    MD5Digest           check;
    std::string         checksum;
    LZMACompressor      c;
    LZMADecompressor    d;

    CLASSLIB_TEST_ASSERT_EQUAL(data.size(), 322814);

    for (int i = 0; i < NUM; ++i)
    {
	IOSize csize, dsize;
        c.preset(tests[i].level);
        c.extreme(tests[i].extreme);
        c.reset();
	CLASSLIB_TEST_ASSERT(c.more());
	CLASSLIB_TEST_ASSERT_EQUAL(c.preset(), tests[i].level);
	CLASSLIB_TEST_ASSERT_EQUAL(c.extreme(), tests[i].extreme);
	c.input(data.data(), data.size());
	c.finish();
	csize = c.compress(&comp[0], comp.size());
	CLASSLIB_TEST_ASSERT(c.finished());
	CLASSLIB_TEST_ASSERT_EQUAL(csize, tests[i].csize);
	CLASSLIB_TEST_ASSERT_EQUAL(c.in(), IOOffset (data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(c.out(), IOOffset (csize));

	check.reset();
	check.update(&comp[0], csize);
	checksum = check.format();
	CLASSLIB_TEST_ASSERT_EQUAL(check.format(), tests[i].checksum);

	d.reset();
	CLASSLIB_TEST_ASSERT(d.more());
	d.input(&comp[0], csize);
	dsize = d.decompress(&decomp[0], decomp.size());
	CLASSLIB_TEST_ASSERT(d.finished());
	CLASSLIB_TEST_ASSERT(d.rest() == 0);
	CLASSLIB_TEST_ASSERT(! memcmp(data.data(), &decomp[0], data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(dsize, data.size());
	CLASSLIB_TEST_ASSERT_EQUAL(d.out(), IOOffset (data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(d.in(), IOOffset (csize));
    }

    return 0;
}
