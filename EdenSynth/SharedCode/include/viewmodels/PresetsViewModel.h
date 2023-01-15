#pragma once
#include <memory>
#include "ViewModel.h"

namespace eden_vst {
class PresetManager;

namespace viewmodels {
class PresetsViewModel : public ViewModel {
public:
  explicit PresetsViewModel(std::unique_ptr<PresetManager>);

private:
  std::unique_ptr<PresetManager> _presetManager;
};
}  // namespace viewmodels
}  // namespace eden_vst
