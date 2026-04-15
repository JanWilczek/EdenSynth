#include "gtest/gtest.h"
#include "viewmodels/PresetsViewModel.h"
#include "PresetManager.h"

namespace eden::plugin_test {
using eden::plugin::PresetLoadingResult;
using eden::plugin::PresetSavingResult;

class FakePresetManager : public eden::plugin::PresetManager {
  PresetSavingResult saveCurrentPreset(const std::string&) override {
    return PresetSavingResult();
  }
  PresetSavingResult saveOrOverwriteCurrentPreset(const std::string&) override {
    return PresetSavingResult();
  }
  PresetLoadingResult loadPreset(const eden::plugin::Preset&) override {
    return PresetLoadingResult();
  }

  const eden::plugin::Presets::Container& presets() override {
    return _presets.presets();
  }

private:
  eden::plugin::Presets _presets;
};
}  // namespace eden::plugin_test
