#include <string>

#include "absl/status/status.h"

absl::Status Error(std::string message, int line, int column);
