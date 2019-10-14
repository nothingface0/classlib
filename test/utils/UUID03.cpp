#include "test/Test.h" 
#include "classlib/utils/UUID.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <sstream>

using namespace lat;
typedef lat::UUID TestUUID;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    TestUUID uuid = TestUUID::generateRandom();
    std::ostringstream str; str << uuid;
    CLASSLIB_TEST_ASSERT_EQUAL(TestUUID(str.str()), uuid);
    return 0;
}
