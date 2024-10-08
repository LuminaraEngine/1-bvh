#pragma once

#include <stdexcept>
#include <string>

class AssertionFailedException : public std::runtime_error {
public:
  AssertionFailedException(const std::string& message)
    : std::runtime_error(message) {}
};

#define assert(condition, message) \
  if (!(condition)) { \
    throw AssertionFailedException(message); \
  }
