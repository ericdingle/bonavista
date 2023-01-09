#include "util/error.h"

absl::Status Error(std::string message, int line, int column) {
  return absl::InvalidArgumentError(
      absl::StrCat(message, " at ", line, ":", column, "."));
}
