#pragma once
#include <string>

namespace eden::plugin {
class ErrorDialogListener {
public:
  virtual void showErrorDialogWithMessage(const std::string& message) = 0;
};
}  // namespace eden::plugin
