#include "util/status.h"

#include <sstream>

Status::Status() : Status("", 0, 0) {
}

Status::Status(const std::string& message, int line, int column)
    : message_(message), line_(line), column_(column) {
}

bool Status::error() const {
  return !message_.empty();
}

std::string Status::message() const {
  return message_;
}

std::string Status::ToString() const {
  if (message_.empty()) {
    return "";
  }

  std::ostringstream stream;
  stream << message_ << " at line " << line_ << ", column " << column_ << ".";
  return stream.str();
}
