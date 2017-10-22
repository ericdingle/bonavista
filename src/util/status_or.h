#ifndef UTIL_STATUS_OR_H_
#define UTIL_STATUS_OR_H_

#include "util/status.h"

template <typename Value>
class StatusOr {
 public:
  StatusOr(const Status& status);
  StatusOr(const Value& value);
  StatusOr(Value&& value);

  StatusOr(const StatusOr& s);
  StatusOr(StatusOr&& s);
  StatusOr operator=(const StatusOr&) = delete;
  StatusOr operator=(StatusOr&&) = delete;

  ~StatusOr() = default;

  bool error() const;
  const Status& status() const;
  const Value& value() const;
  Value&& value();

 private:
  Status status_;
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
inline StatusOr<Value>::StatusOr(const StatusOr& s)
    : status_(s.status_), value_(s.value_) {
}

template <typename Value>
inline StatusOr<Value>::StatusOr(StatusOr&& s)
    : status_(s.status_), value_(std::move(s.value_)) {
}

template <typename Value>
inline bool StatusOr<Value>::error() const {
  return status_.error();
}

template <typename Value>
inline const Status& StatusOr<Value>::status() const {
  return status_;
}

template <typename Value>
inline const Value& StatusOr<Value>::value() const {
  return value_;
}

template <typename Value>
inline Value&& StatusOr<Value>::value() {
  return std::move(value_);
}

#define TOKEN_CONCAT_IMPL(a, b) a##b
#define TOKEN_CONCAT(a, b) TOKEN_CONCAT_IMPL(a, b)

#define ASSIGN_OR_RETURN_IMPL(status_or, lhs, expr) \
  auto status_or = (expr);                          \
  if (status_or.error()) {                          \
    return status_or.status();                      \
  }                                                 \
  lhs = status_or.value()

#define ASSIGN_OR_RETURN(lhs, expr) \
  ASSIGN_OR_RETURN_IMPL(TOKEN_CONCAT(_error_or, __COUNTER__), lhs, expr)

#endif  // UTIL_STATUS_OR_H_
