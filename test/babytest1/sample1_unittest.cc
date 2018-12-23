
#include "sample1.h"
#include "gtest/gtest.h"

TEST(HelloworldTest, Zero) {
  EXPECT_EQ(1, helloworld(0));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


