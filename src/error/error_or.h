#ifndef ERROR_ERROR_OR_H_
#define ERROR_ERROR_OR_H_

#include <assert.h>
#include "error/error.h"

template <typename Value>
class ErrorOr {
 public:
  ErrorOr(const Error& error);
  ErrorOr(const Value& value);
  ErrorOr(Value&& value);

  ErrorOr(const ErrorOr& e);
  ErrorOr(ErrorOr&& e);
  ErrorOr operator=(const ErrorOr&) = delete;
  ErrorOr operator=(ErrorOr&&) = delete;

  ~ErrorOr();

  bool is_error() const;
  const Error& error() const;
  const Value& value() const;
  Value&& value();

 private:
  bool is_error_;
  union {
    Error error_;
    Value value_;
  };
};

template <typename Value>
inline ErrorOr<Value>::ErrorOr(const Error& error) : is_error_(true), error_(error) {
}

template <typename Value>
inline ErrorOr<Value>::ErrorOr(const Value& value) : is_error_(false), value_(value) {
}

template <typename Value>
inline ErrorOr<Value>::ErrorOr(Value&& value) : is_error_(false), value_(std::move(value)) {
}

template <typename Value>
inline ErrorOr<Value>::ErrorOr(const ErrorOr& e) : is_error_(e.is_error_) {
  if (is_error_) {
    new (&error_) Error(e.error());
  } else {
    new (&value_) Value(e.value());
  }
}

template <typename Value>
inline ErrorOr<Value>::ErrorOr(ErrorOr&& e) : is_error_(e.is_error_) {
  if (is_error_) {
    new (&error_) Error(e.error());
  } else {
    new (&value_) Value(std::move(e.value()));
  }
}

template <typename Value>
inline ErrorOr<Value>::~ErrorOr() {
  if (is_error_) {
    error_.~Error();
  } else {
    value_.~Value();
  }
}

template <typename Value>
inline bool ErrorOr<Value>::is_error() const {
  return is_error_;
}

template <typename Value>
inline const Error& ErrorOr<Value>::error() const {
  assert(is_error_);
  return error_;
}

template <typename Value>
inline const Value& ErrorOr<Value>::value() const {
  assert(!is_error_);
  return value_;
}

template <typename Value>
inline Value&& ErrorOr<Value>::value() {
  assert(!is_error_);
  return std::move(value_);
}

#define TOKEN_CONCAT_IMPL(a, b) a##b
#define TOKEN_CONCAT(a, b) TOKEN_CONCAT_IMPL(a, b)

#define ASSIGN_OR_RETURN_ERROR_IMPL(error_or, lhs, expr) \
  auto error_or = (expr);                                \
  if (error_or.is_error()) {                             \
    return error_or.error();                             \
  }                                                      \
  lhs = error_or.value()

#define ASSIGN_OR_RETURN_ERROR(lhs, expr) \
  ASSIGN_OR_RETURN_ERROR_IMPL(TOKEN_CONCAT(_error_or, __COUNTER__), lhs, expr)

#endif  // ERROR_ERROR_OR_H_
