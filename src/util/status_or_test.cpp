#include <memory>
#include "util/status_or.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(StatusOrTest, Status) {
  StatusOr<int> status_or(Status("test", 1, 1));
  EXPECT_NOT_OK(status_or);
  EXPECT_STATUS(status_or.status(), "test", 1, 1);
}

TEST(StatusOrTest, ValuePOD) {
  const StatusOr<int> status_or(1);
  EXPECT_OK(status_or);
  EXPECT_EQ(1, status_or.value());
}

TEST(StatusOrTest, ValueUniquePtr) {
  StatusOr<std::unique_ptr<int>> status_or(std::make_unique<int>(1));
  EXPECT_OK(status_or);
  EXPECT_EQ(1, *status_or.value());
}

TEST(StatusOrTest, AssignOrReturnStatus) {
  auto func = []() -> Status {
    ASSIGN_OR_RETURN(
        std::unique_ptr<int> i,
        StatusOr<std::unique_ptr<int>>(std::make_unique<int>(1)));
    EXPECT_EQ(1, *i);
    ASSIGN_OR_RETURN(int j, StatusOr<int>(Status("yes", 1, 1)));
    (void)j;  // Fix unused warning.
    return Status("no", 1, 1);
  };
  EXPECT_STATUS(func(), "yes", 1, 1);
}
