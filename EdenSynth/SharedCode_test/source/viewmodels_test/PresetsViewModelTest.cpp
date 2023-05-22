#include "gtest/gtest.h"
#include "viewmodels/PresetsViewModel.h"
#include "PresetManager.h"

namespace eden_vst_test {
class FakePresetManager : public eden_vst::PresetManager {
  void saveCurrentPreset(std::function<void(const std::string&)>) override {}
  void loadPreset(const std::string&) override {}
  std::vector<std::string> presets() const override { return {}; }
};

TEST(Foo, Bar) {
  FakePresetManager manager{};
  eden_vst::viewmodels::PresetsViewModel viewModel{manager};
  ASSERT_FALSE(true);
}
}  // namespace eden_vst_test
