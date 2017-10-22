#include "error/error.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(ErrorTest, ToString) {
  Error e("blah", 10, 20);
  EXPECT_EQ("blah", e.message());
  EXPECT_EQ("Error: blah at line 10, column 20.", e.ToString());
}
