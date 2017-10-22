#ifndef ERROR_ERROR_H_
#define ERROR_ERROR_H_

#include <string>

class Error {
 public:
  Error(int line, int column, const std::string& message);
  Error(const Error&) = default;
  Error& operator=(const Error&) = default;
  ~Error() = default;

  std::string message() const;
  std::string ToString() const;

 private:
  const int line_;
  const int column_;
  const std::string message_;
};

#endif  // ERROR_ERROR_H_
