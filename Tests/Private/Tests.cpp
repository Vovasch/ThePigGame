#include "Tests.h"

#include "gtest/gtest.h"
/*#include "src/gtest-all.cc" //Do not do this in Unreal 4.20. It will cause some errors because internally google test adds windows.h which conflicts to other headers that CoreMinimal.h depends.
#include "src/gmock-all.cc"
#include "gtest/gtest-assertion-result.h"
#include "gtest/gtest-message.h"
#include "gtest/gtest-test-part.h"
#include "src/gtest_main.cc"
#include "gtest/internal/gtest-internal.h"
#include "gtest/internal/gtest-port.h"*/

class GivenASampleTest : public ::testing::Test {

};

TEST_F(GivenASampleTest, WhenRunningSampleTestShouldPass)
{
    ASSERT_TRUE(true);
}