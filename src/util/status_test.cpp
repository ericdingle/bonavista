#include "util/status.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(StatusTest, DefaultConstructor) {
  Status status;
  EXPECT_OK(status);
  EXPECT_STATUS(status, "", 0, 0);
}

TEST(StatusTest, Constructor) {
  Status status("blah", 10, 20);
  EXPECT_NOT_OK(status);
  EXPECT_STATUS(status, "blah", 10, 20);
}

TEST(StatusTest, CopyConstructor) {
  Status status1("a", 1, 1);
  Status status2(status1);
  EXPECT_STATUS(status2, "a", 1, 1);
}

TEST(StatusTest, ReturnIfError) {
  auto func = []() -> Status {
    RETURN_IF_ERROR(Status());
    RETURN_IF_ERROR(Status("yes", 1, 1));
    return Status("no", 1, 1);
  };
  EXPECT_STATUS(func(), "yes", 1, 1);
}
