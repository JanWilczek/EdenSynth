#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <optional>
#include "Preset.h"

// NEW PRESETS
namespace eden::plugin {
/// <summary>
///
/// </summary>
class Presets {
public:
  using Container = std::vector<Preset>;

  explicit Presets(std::filesystem::path systemPresetsPath,
                   std::filesystem::path userPresetsPath);

  [[nodiscard]] const Container& presets();
  [[nodiscard]] bool containsUserPreset(const std::string& presetName);
  [[nodiscard]] const Preset& createPreset(const std::string& name);
  void scanForPresets();

private:
  std::filesystem::path _systemPresetsPath;
  std::filesystem::path _userPresetsPath;
  Container _presets;
};
}  // namespace eden::plugin
