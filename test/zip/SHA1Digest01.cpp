#include "test/Test.h" 
#include "classlib/zip/SHA1Digest.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;
struct SHA1Check
{
    const char *checksum;
    const char *input;
};

const SHA1Check tests[] = {
    { "2ef7bde608ce5404e97d5f042f95f89f1c232871", "Hello World!\0" },
    { "2ef7bde608ce5404e97d5f042f95f89f1c232871", "Hello\0 World!\0" },
    { "2ef7bde608ce5404e97d5f042f95f89f1c232871", "H\0e\0l\0l\0o\0 \0W\0o\0r\0l\0d\0!\0" }
};

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const int NUM = sizeof(tests) / sizeof(tests[0]);
    SHA1Digest t;

    for (int i = 0; i < NUM; ++i)
    {
	const char *p = tests[i].input;
	SHA1Digest t2;
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
    CLASSLIB_TEST_ASSERT_EQUAL(t.format(), "b19b3c8ab7ade3b14a7478574766f0578c940c7f");
    return 0;
}
