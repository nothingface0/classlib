#include "test/Test.h" 
#include "classlib/utils/UUID.h"
#include "classlib/utils/UUIDError.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <sstream>

using namespace lat;
typedef lat::UUID TestUUID;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    bool ok;

    try { ok = true; TestUUID("7087E77F-8789-D911-9FCB-0030482B37E8"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, true);

    try { ok = true; TestUUID("84949cc5-4701-4a84-895b-354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, true);

    try { ok = true; TestUUID("84949CC5-4701-4A84-895B-354C584A981B"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, true);

    try { ok = true; TestUUID("84949cc5-4701-4a84-895b-354c584a981bc"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc5-4701-4a84-895b-354c584a981"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc5x4701-4a84-895b-354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc504701-4a84-895b-354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc5-470104a84-895b-354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc5-4701-4a840895b-354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc5-4701-4a84-895b0354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("g4949cc5-4701-4a84-895b-354c584a981b"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);

    try { ok = true; TestUUID("84949cc5-4701-4a84-895b-354c584a981g"); }
    catch (UUIDError &) { ok = false; } CLASSLIB_TEST_ASSERT_EQUAL(ok, false);
    return 0;
}
