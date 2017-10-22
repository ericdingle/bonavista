#include <memory>
#include "util/status_or.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(StatusOrTest, Status) {
  StatusOr<int> status_or(Status("test", 1, 1));
  EXPECT_TRUE(status_or.error());
  const Status& status = status_or.status();
  EXPECT_EQ("test", status.message());
}

TEST(StatusOrTest, ValuePOD) {
  const StatusOr<int> status_or(1);
  EXPECT_FALSE(status_or.error());
  int a = status_or.value();
  EXPECT_EQ(1, a);
}

TEST(StatusOrTest, ValueUniquePtr) {
  StatusOr<std::unique_ptr<int>> status_or(std::unique_ptr<int>(new int(1)));
  EXPECT_FALSE(status_or.error());
  std::unique_ptr<int> i = status_or.value();
  EXPECT_EQ(1, *i);
}

TEST(StatusOrTest, AssignOrReturnStatus) {
  auto func = []() -> Status {
    ASSIGN_OR_RETURN(
        std::unique_ptr<int> i,
        StatusOr<std::unique_ptr<int>>(std::unique_ptr<int>(new int(1))));
    EXPECT_EQ(1, *i);
    ASSIGN_OR_RETURN(int j, StatusOr<int>(Status("yes", 1, 1)));
    (void)j;  // Fix unused warning.
    return Status("no", 1, 1);
  };
  EXPECT_EQ("yes", func().message());
}
