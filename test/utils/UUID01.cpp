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
    std::ostringstream str;
    str << TestUUID();
    CLASSLIB_TEST_ASSERT_EQUAL(str.str(), "00000000-0000-0000-0000-000000000000");
    return 0;
}
