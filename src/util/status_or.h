#ifndef UTIL_STATUS_OR_H_
#define UTIL_STATUS_OR_H_

#include "util/status.h"

template <typename Value>
class StatusOr {
 public:
  StatusOr(const Status& status);
  StatusOr(const Value& value);
  StatusOr(Value&& value);

  StatusOr(const StatusOr& s) = default;
  StatusOr(StatusOr&& s) = default;
  StatusOr operator=(const StatusOr&) = delete;

  ~StatusOr() = default;

  bool ok() const;
  const Status& status() const { return status_; }
  const Value& value() const { return value_; }
  Value&& value() { return std::move(value_); }

 private:
  const Status status_;
  Value value_;
};

template <typename Value>
inline StatusOr<Value>::StatusOr(const Status& status) : status_(status) {
}

template <typename Value>
inline StatusOr<Value>::StatusOr(const Value& value) : value_(value) {
}

template <typename Value>
inline StatusOr<Value>::StatusOr(Value&& value) : value_(std::move(value)) {
}

template <typename Value>
inline bool StatusOr<Value>::ok() const {
  return status_.ok();
}

#define TOKEN_CONCAT_IMPL(a, b) a##b
#define TOKEN_CONCAT(a, b) TOKEN_CONCAT_IMPL(a, b)

#define ASSIGN_OR_RETURN_IMPL(status_or, lhs, expr) \
  auto status_or = (expr);                          \
  if (!status_or.ok()) {                            \
    return status_or.status();                      \
  }                                                 \
  lhs = status_or.value()

#define ASSIGN_OR_RETURN(lhs, expr) \
  ASSIGN_OR_RETURN_IMPL(TOKEN_CONCAT(_status_or, __COUNTER__), lhs, expr)

#endif  // UTIL_STATUS_OR_H_
