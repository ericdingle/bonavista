#ifndef ERROR_ERROR_H_
#define ERROR_ERROR_H_

#include <string>

class Error {
 public:
  Error(const std::string& message, int line, int column);
  Error(const Error&) = default;
  Error& operator=(const Error&) = default;
  ~Error() = default;

  std::string message() const;
  std::string ToString() const;

 private:
  const std::string message_;
  const int line_;
  const int column_;
};

#endif  // ERROR_ERROR_H_
