#include "test/Test.h" 
#include "classlib/zip/ZCompressor.h"
#include "classlib/zip/ZDecompressor.h"
#include "classlib/zip/MD5Digest.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Signal.h"
#include <vector>

using namespace lat;
struct ZCompressCheck
{
    unsigned level;
    IOSize csize;
    const char *checksum;
};

const ZCompressCheck tests[] = {
    { 1, 148445, "cbab27ddd1522dcde6a823ea2e11a88e" },
    { 2, 141781, "9cc8316d5217047faf6974273af7a039" },
    { 3, 134959, "764e190948ae4db896a437edcbdb8606" },
    { 4, 132227, "d4c0d245692d25cfa5106e4af79ba21f" },
    { 5, 127151, "5f6c8da8943ddabc3b732120f3526d9e" },
    { 6, 124852, "50a92d8d5c758c077ff8d7f0b70caf21" },
    { 7, 124606, "5b51da1c74294a283c4f148e94a3e6ee" },
    { 8, 124535, "5700e7c457d7981fe69106bf1f7380bc" },
    { 9, 124535, "5700e7c457d7981fe69106bf1f7380bc" }
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
    ZCompressor         c;
    ZDecompressor       d;

    CLASSLIB_TEST_ASSERT_EQUAL(data.size(), 322814);

    for (int i = 0; i < NUM; ++i)
    {
	IOSize csize, dsize;
        c.level(tests[i].level);
        c.reset();
	CLASSLIB_TEST_ASSERT(c.more());
	CLASSLIB_TEST_ASSERT_EQUAL(c.level(), tests[i].level);
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
	d.input(&comp[0], csize+1);
	dsize = d.decompress(&decomp[0], decomp.size());
	CLASSLIB_TEST_ASSERT(d.finished());
	CLASSLIB_TEST_ASSERT(d.rest() == 1);
	CLASSLIB_TEST_ASSERT(! memcmp(data.data(), &decomp[0], data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(dsize, data.size());
	CLASSLIB_TEST_ASSERT_EQUAL(d.out(), IOOffset (data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(d.in(), IOOffset (csize));
    }

    return 0;
}
