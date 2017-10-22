#include "util/status.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(StatusTest, Default) {
  Status status;
  EXPECT_FALSE(status.error());
  EXPECT_EQ("", status.message());
  EXPECT_EQ("", status.ToString());
}

TEST(StatusTest, ToString) {
  Status status("blah", 10, 20);
  EXPECT_TRUE(status.error());
  EXPECT_EQ("blah", status.message());
  EXPECT_EQ("blah at line 10, column 20.", status.ToString());
}

TEST(StatusTest, ReturnIfError) {
  auto func = []() -> Status {
    RETURN_IF_ERROR(Status());
    RETURN_IF_ERROR(Status("yes", 1, 1));
    return Status("no", 1, 1);
  };
  EXPECT_EQ("yes", func().message());
}
