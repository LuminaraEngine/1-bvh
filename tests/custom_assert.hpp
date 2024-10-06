#pragma once

#include <string>
#include <stdexcept>

void custom_assert(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}
