#ifndef UTIL_STATUS_H_
#define UTIL_STATUS_H_

#include <string>

class Status {
 public:
  Status();
  Status(const std::string& message, int line, int column);
  Status(const Status&) = default;
  Status& operator=(const Status&) = delete;
  ~Status() = default;

  bool error() const;
  std::string message() const;
  std::string ToString() const;

 private:
  const std::string message_;
  const int line_;
  const int column_;
};

#define RETURN_IF_ERROR(expr) \
  do {                        \
    Status _status = (expr);  \
    if (_status.error()) {     \
      return _status;          \
    }                         \
  } while (false)

#endif  // UTIL_STATUS_H_
