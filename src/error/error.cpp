#include "error/error.h"

#include <sstream>

Error::Error(int line, int column, const std::string& message)
    : line_(line), column_(column), message_(message) {
}

std::string Error::message() const {
  return message_;
}

std::string Error::ToString() const {
  std::ostringstream stream;
  stream << "Error: " << message_ << " at line " << line_ << ", column "
         << column_ << ".";
  return stream.str();
}
