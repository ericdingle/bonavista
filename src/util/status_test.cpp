#include "util/status.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class StatusTest : public testing::Test {
 protected:
  void ExpectStatus(const Status& status, const std::string& message, int line,
                    int column) {
    EXPECT_EQ(message, status.message());
    EXPECT_EQ(line, status.line());
    EXPECT_EQ(column, status.column());
  }

  void ExpectStatus(const Status& status, const std::string& message, int line,
                    int column, bool error, const std::string& to_string) {
    ExpectStatus(status, message, line, column);
    EXPECT_EQ(error, status.error());
    EXPECT_EQ(to_string, status.ToString());
  }
};

TEST_F(StatusTest, DefaultConstructor) {
  Status status;
  ExpectStatus(status, "", 0, 0, false, "");
}

TEST_F(StatusTest, Constructor) {
  Status status("blah", 10, 20);
  ExpectStatus(status, "blah", 10, 20, true, "blah at line 10, column 20.");
}

TEST_F(StatusTest, CopyConstructor) {
  Status status1("a", 1, 1);
  Status status2(status1);
  ExpectStatus(status2, "a", 1, 1);
}

TEST_F(StatusTest, ReturnIfError) {
  auto func = []() -> Status {
    RETURN_IF_ERROR(Status());
    RETURN_IF_ERROR(Status("yes", 1, 1));
    return Status("no", 1, 1);
  };
  ExpectStatus(func(), "yes", 1, 1);
}
