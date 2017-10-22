#include <memory>
#include "error/error_or.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(ErrorOrTest, Error) {
  ErrorOr<int> error_or(Error("test", 1, 1));
  EXPECT_TRUE(error_or.is_error());
  const Error& error = error_or.error();
  EXPECT_EQ("test", error.message());
}

TEST(ErrorOrTest, ErrorDeath) {
  ErrorOr<int> error_or(Error("test", 1, 1));
  EXPECT_DEATH(error_or.value(), "Assertion `!is_error_' failed");
}

TEST(ErrorOrTest, ValuePOD) {
  const ErrorOr<int> error_or(1);
  EXPECT_FALSE(error_or.is_error());
  int a = error_or.value();
  EXPECT_EQ(1, a);
}

TEST(ErrorOrTest, ValueUniquePtr) {
  ErrorOr<std::unique_ptr<int>> error_or(std::unique_ptr<int>(new int(1)));
  EXPECT_FALSE(error_or.is_error());
  std::unique_ptr<int> i = error_or.value();
  EXPECT_EQ(1, *i);
}

TEST(ErrorOrTest, ValueDeath) {
  ErrorOr<int> error_or(1);
  EXPECT_DEATH(error_or.error(), "Assertion `is_error_' failed");
}

ErrorOr<std::unique_ptr<int>> ReturnInt(int i) {
  return std::unique_ptr<int>(new int(i));
}

ErrorOr<int> ReturnError(const std::string& message) {
  return Error(message, 1, 1);
};

TEST(ErrorOrTest, AssignOrReturnError) {
  auto func = []() -> Error {
    ASSIGN_OR_RETURN_ERROR(std::unique_ptr<int> i, ReturnInt(1));
    EXPECT_EQ(1, *i);
    ASSIGN_OR_RETURN_ERROR(int j, ReturnError("yes"));
    (void)j;  // Address unused warning.
    return Error("no", 1, 1);
  };
  EXPECT_EQ("yes", func().message());
}
