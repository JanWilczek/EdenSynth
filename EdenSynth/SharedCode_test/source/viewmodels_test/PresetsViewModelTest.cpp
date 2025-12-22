#include "gtest/gtest.h"
#include "viewmodels/PresetsViewModel.h"
#include "PresetManager.h"

namespace eden_vst_test {
using eden_vst::PresetLoadingResult;
using eden_vst::PresetSavingResult;

class FakePresetManager : public eden_vst::PresetManager {
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
}  // namespace eden_vst_test
