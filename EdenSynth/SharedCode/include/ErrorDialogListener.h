#pragma once
#include <string>

namespace eden_vst {
class ErrorDialogListener {
public:
  virtual void showErrorDialogWithMessage(const std::string& message) = 0;
};
}  // namespace eden_vst
