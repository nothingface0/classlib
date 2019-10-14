#include "test/Test.h" 
#include "classlib/zip/LZOCompressor.h"
#include "classlib/zip/LZODecompressor.h"
#include "classlib/zip/MD5Digest.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Signal.h"
#include <vector>

using namespace lat;
struct LZOCompressCheck
{
    bool optimise;
    unsigned level;
    IOSize csize;
    const char *checksum;
};

const LZOCompressCheck tests[] = {
    { false, LZOConstants::LZO1  << 24 | 1 << 8,        204219, "a81430fcfae88dca2f9bae95aa6c1854" },
    { false, LZOConstants::LZO1  << 24 | 99 << 8,       170099, "01b6a4dfa2e95805f4784e8e74463f98" },
    { false, LZOConstants::LZO1A << 24 | 1 << 8,        202553, "2e5dd7e6131d4e539e1572d9d4481684" },
    { false, LZOConstants::LZO1A << 24 | 99 << 8,       169599, "b9e599f0b48321c593a9090910afd14d" },
    { false, LZOConstants::LZO1B << 24 | 1 << 8,        193130, "080c5ee1c0eb7c6ae7c920a2c4bce42d" },
    { false, LZOConstants::LZO1B << 24 | 2 << 8,        184153, "b0126d30ac632e5aa06e2df23bb7f08c" },
    { false, LZOConstants::LZO1B << 24 | 3 << 8,        179238, "acc3256917a158975178ad409151c9ac" },
    { false, LZOConstants::LZO1B << 24 | 4 << 8,        185406, "7560c891d68a11ab81090d54652629a3" },
    { false, LZOConstants::LZO1B << 24 | 5 << 8,        180383, "b41e8b788947c7078bb901f55e987101" },
    { false, LZOConstants::LZO1B << 24 | 6 << 8,        178034, "6ba5abc1e4aae2db8bc5270d8e0122f7" },
    { false, LZOConstants::LZO1B << 24 | 7 << 8,        176206, "c910ef690ea12425c4037b255e57d4b4" },
    { false, LZOConstants::LZO1B << 24 | 8 << 8,        173409, "3707e14a294d03e7da00ab2fa39e737c" },
    { false, LZOConstants::LZO1B << 24 | 9 << 8,        173496, "642accfce0595b7871d4737af99c7674" },
    { false, LZOConstants::LZO1B << 24 | 99 << 8,       167267, "62d8db482e777663b52577afa26253fd" },
    { false, LZOConstants::LZO1B << 24 | 999 << 8,      138939, "51171a387d6c6504ad1b28024f4c293a" },
    { false, LZOConstants::LZO1C << 24 | 1 << 8,        196478, "6d45d78783a9d3f7e194381cdac31db1" },
    { false, LZOConstants::LZO1C << 24 | 2 << 8,        188871, "a4bc16e278f5718f1e1cb2a8df2bdd3a" },
    { false, LZOConstants::LZO1C << 24 | 3 << 8,        183897, "ed6a3d7665c5df9a28642b7fcf318684" },
    { false, LZOConstants::LZO1C << 24 | 4 << 8,        186433, "d9ec6ac3260f63f2b4b58f04547fefc3" },
    { false, LZOConstants::LZO1C << 24 | 5 << 8,        181863, "5870f89294a9eab4f90c54aa1063329e" },
    { false, LZOConstants::LZO1C << 24 | 6 << 8,        179992, "7fa99a2d13d0104ab5c87097b76db98a" },
    { false, LZOConstants::LZO1C << 24 | 7 << 8,        176369, "f2a8091f0e1ee9f9c57a8ad1a0f7e218" },
    { false, LZOConstants::LZO1C << 24 | 8 << 8,        174938, "20f0398d6114223184fa002cbf2504af" },
    { false, LZOConstants::LZO1C << 24 | 9 << 8,        174695, "6d4230098f4e01ed12c01e5d8c497d7c" },
    { false, LZOConstants::LZO1C << 24 | 99 << 8,       169075, "7a7b794f2c15b900b168186fd94244ed" },
    { false, LZOConstants::LZO1C << 24 | 999 << 8,      147771, "a865727d276f90903afb8bd66da95745" },
    { false, LZOConstants::LZO1F << 24 | 1 << 8,        203531, "74794e4220759593d4d4ebef3e4a3966" },
    { false, LZOConstants::LZO1F << 24 | 999 << 8,      157609, "26f95ad4afe55c287d3bf50601c9d596" },
    { false, LZOConstants::LZO1X << 24 | 1 << 8,        196855, "6c1cd01af01aaea0d40b6f1df5e85dbc" },
    { true , LZOConstants::LZO1X << 24 | 1 << 8,        196856, "bfb73c963ce3959b5ba279df98b2cb9c" },
    { false, LZOConstants::LZO1X << 24 | 1 << 8 | 11,   212707, "1bf412010bdebee3d5acaad7aa6c5652" },
    { true , LZOConstants::LZO1X << 24 | 1 << 8 | 11,   212726, "c01ce007137e526a62156de461e1eb04" },
    { false, LZOConstants::LZO1X << 24 | 1 << 8 | 12,   205480, "a4e52b3ccc1b27f521e1a7beca32825f" },
    { true , LZOConstants::LZO1X << 24 | 1 << 8 | 12,   205252, "4a600fa56fe8674d580adc3346b0b15f" },
    { false, LZOConstants::LZO1X << 24 | 1 << 8 | 15,   195042, "2a74b360a7be2e90e478cdbd53ea1dd8" },
    { true , LZOConstants::LZO1X << 24 | 1 << 8 | 15,   194903, "5f4946ea2909e6f7b0610c448428aeb0" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 1,  173428, "23a2c68b8b1c0c8cbd9d0f41ed0be502" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 1,  173428, "2dbf6bf2346257c783af84bc76ac56ca" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 2,  166057, "39aced34158e3232f1384b6eea72f68d" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 2,  166057, "581f210f1e2b45b080ec8dd39639a8f8" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 3,  160907, "c2eff8f7319a01672a3625c9a89c383c" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 3,  160907, "9d5f77aab2e3af54eb6c7138d3904d48" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 4,  157571, "e88d3717e1bbace4d610180f38338539" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 4,  157571, "89eb58d40d1ca488e22a73a70bc20c0c" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 5,  150570, "6af2c444e9f073476585f43e64c07c45" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 5,  150570, "8932da4eeb3647eeccbdaacd44844861" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 6,  147720, "6ebe269b97b6fe61b39744da198976de" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 6,  147720, "22df7fe1df6e206aad95ea0ae3297870" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 7,  146202, "1496b81195b23a07a90e4810b3fbd699" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 7,  146202, "e927ad3c11b7189c41fc7141acd42c6d" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 8,  143367, "d482e13c9a63560ed6d29078aad12346" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 8,  143367, "e28ac99f88fa2d8890e87b99de74dd6d" },
    { false, LZOConstants::LZO1X << 24 | 999 << 8 | 9,  143367, "d482e13c9a63560ed6d29078aad12346" },
    { true , LZOConstants::LZO1X << 24 | 999 << 8 | 9,  143367, "e28ac99f88fa2d8890e87b99de74dd6d" },
    { false, LZOConstants::LZO1Y << 24 | 1 << 8,        202203, "a416bd8912203d3fc234d1d0c407a0ce" },
    { true , LZOConstants::LZO1Y << 24 | 1 << 8,        202198, "3cec59f5b71bbafc079cfa9c6d00cff7" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 1,  182630, "5415e5fb3b066ddc7e301fe523b06ea5" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 1,  182630, "194676932d84c85146a2e51c4fcb575e" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 2,  173911, "b582db653537a7f8c48d5930a3ba1e73" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 2,  173911, "a13186dc09b179f8e939d0cdaacd8919" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 3,  166970, "6764b27d86b90e3180beee9c1d7a7fa5" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 3,  166970, "53ff2091f7b5d9b2d976fba798b3070a" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 4,  162978, "65fddd50e02a97aa47b7826b087f1b63" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 4,  162978, "7916723a51307c809c742bb0f09c6ed0" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 5,  154259, "e3183884e16a656206e25877ca5ead33" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 5,  154259, "b64fef7fbcd9f3c73f8865c88ff834d8" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 6,  150322, "1ea1e3ee3b550d48b5a6d18a983aad58" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 6,  150322, "d977bd382470ecb8856172791c72285e" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 7,  148468, "f0f8f5a09d5dd34c39e5bb120da68362" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 7,  148468, "46436bea5e4be0edda1fcd512a750cbc" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 8,  146175, "bed10816732ed71b53061e665396738e" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 8,  146175, "4d4f85672182748efe5bcd6c0f5be02e" },
    { false, LZOConstants::LZO1Y << 24 | 999 << 8 | 9,  146175, "bed10816732ed71b53061e665396738e" },
    { true , LZOConstants::LZO1Y << 24 | 999 << 8 | 9,  146175, "4d4f85672182748efe5bcd6c0f5be02e" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 1,  175268, "9e887680ab37af58363170609abc0a3c" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 2,  167747, "a57662eef61e73c5a6cfbddca202b3c9" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 3,  162296, "7907aacd20991088220bcc5706e2671f" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 4,  158811, "d8fe658fa89288eef4649c8889a0444e" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 5,  151485, "87703098eeac7d2ad98db9d2a98e5020" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 6,  148412, "16170499c91540ea37f487b96e5ba47d" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 7,  146830, "c8b6c2d33091de8739be1333ec9f29ae" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 8,  144114, "9ca74e66619694dbded22262590d3bcc" },
    { false, LZOConstants::LZO1Z << 24 | 999 << 8 | 9,  144114, "9ca74e66619694dbded22262590d3bcc" },
    { false, LZOConstants::LZO2A << 24 | 999 << 8,      150568, "1ced5b2df9e678acec758eefef23904f" }
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
    LZOCompressor       c;
    LZODecompressor     d;

    CLASSLIB_TEST_ASSERT_EQUAL(data.size(), 322814);

    for (int i = 0; i < NUM; ++i)
    {
	IOSize csize, dsize;
        c.level(tests[i].level);
        c.optimise(tests[i].optimise);
        c.reset();
	CLASSLIB_TEST_ASSERT(c.more());
	CLASSLIB_TEST_ASSERT_EQUAL(c.level(), tests[i].level);
	CLASSLIB_TEST_ASSERT_EQUAL(c.optimise(), tests[i].optimise);
	c.input(data.data(), data.size());
	c.finish();
	csize = c.compress(&comp[0], comp.size());
	CLASSLIB_TEST_ASSERT(c.more());
	CLASSLIB_TEST_ASSERT(c.finished());
	CLASSLIB_TEST_ASSERT_EQUAL(csize, tests[i].csize);
	CLASSLIB_TEST_ASSERT_EQUAL(c.in(), IOOffset (data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(c.out(), IOOffset (csize));

	check.reset();
	check.update(&comp[0], csize);
	checksum = check.format();
	CLASSLIB_TEST_ASSERT_EQUAL(check.format(), tests[i].checksum);

        LZOConstants::Algorithm algo = LZOConstants::Algorithm (tests[i].level >> 24);
        d.algorithm(algo);
        d.reset();
	CLASSLIB_TEST_ASSERT(d.more());
	CLASSLIB_TEST_ASSERT_EQUAL(d.algorithm(), algo);
	d.input(&comp[0], csize);
	dsize = d.decompress(&decomp[0], decomp.size());
	CLASSLIB_TEST_ASSERT(d.more());
	CLASSLIB_TEST_ASSERT(d.finished());
	CLASSLIB_TEST_ASSERT(d.rest() == 0);
	CLASSLIB_TEST_ASSERT(! memcmp(data.data(), &decomp[0], data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(dsize, data.size());
	CLASSLIB_TEST_ASSERT_EQUAL(d.out(), IOOffset (data.size()));
	CLASSLIB_TEST_ASSERT_EQUAL(d.in(), IOOffset (csize));
    }

    return 0;
}
