#include "test/Test.h" 
#include "classlib/zip/MD5Digest.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;
struct MD5Check
{
    const char *checksum;
    const char *input;
};

const MD5Check tests[] = {
    { "ed076287532e86365e841e92bfc50d8c", "Hello World!\0" },
    { "ed076287532e86365e841e92bfc50d8c", "Hello\0 World!\0" },
    { "ed076287532e86365e841e92bfc50d8c", "H\0e\0l\0l\0o\0 \0W\0o\0r\0l\0d\0!\0" }
};

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const int NUM = sizeof(tests) / sizeof(tests[0]);
    MD5Digest t;

    for (int i = 0; i < NUM; ++i)
    {
	const char *p = tests[i].input;
	MD5Digest t2;
	t.reset();
   	do
	{
	    size_t n = strlen(p);
	    t.update(p, n);
	    t2.update(p, n);
	    p += n+1;
	} while (*p);
	CLASSLIB_TEST_ASSERT_EQUAL(t.format(), tests[i].checksum);
	CLASSLIB_TEST_ASSERT_EQUAL(t2.format(), tests[i].checksum);
    }

    Filename    testfile (Filename (__FILE__).directory(), "test.txt");
    FileMapping fmap (new File (testfile));
    IOBuffer    data = fmap.map ();

    t.reset();
    t.update(data.data(), data.size());
    CLASSLIB_TEST_ASSERT_EQUAL(t.format(), "34f7b6e3b4e09828d78bfe18bd1cdc0b");
    return 0;
}
