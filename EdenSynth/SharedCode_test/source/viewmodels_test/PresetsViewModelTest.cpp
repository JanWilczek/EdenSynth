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
  PresetLoadingResult loadPreset(const std::string&) override {
    return PresetLoadingResult();
  }

  virtual std::vector<std::string> presets() const override {
    return std::vector<std::string>();
  }
};
}  // namespace eden_vst_test
