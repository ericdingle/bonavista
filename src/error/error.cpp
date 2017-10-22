#include "error/error.h"

#include <sstream>

Error::Error(const std::string& message, int line, int column)
    : message_(message), line_(line), column_(column) {
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
