#ifndef UTIL_STATUS_MACROS_H_
#define UTIL_STATUS_MACROS_H_

#define RETURN_IF_ERROR(expr) \
  do {                        \
    auto _status = (expr);    \
    if (!_status.ok()) {      \
      return _status;         \
    }                         \
  } while (false)

#define TOKEN_CONCAT_IMPL(a, b) a##b
#define TOKEN_CONCAT(a, b) TOKEN_CONCAT_IMPL(a, b)

#define ASSIGN_OR_RETURN_IMPL(status_or, lhs, expr) \
  auto status_or = (expr);                          \
  if (!status_or.ok()) {                            \
    return status_or.status();                      \
  }                                                 \
  lhs = std::move(status_or.value())

#define ASSIGN_OR_RETURN(lhs, expr) \
  ASSIGN_OR_RETURN_IMPL(TOKEN_CONCAT(_status_or, __COUNTER__), lhs, expr)

#define EXPECT_OK(expr)                             \
  do {                                              \
    auto _status = (expr);                          \
    EXPECT_TRUE(_status.ok()) << _status.message(); \
  } while (false)

#endif  // UTIL_STATUS_MACROS_H_
