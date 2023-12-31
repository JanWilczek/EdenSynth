#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace eden_vst {
class Presets {
public:
  explicit Presets(std::filesystem::path presetsPath);
  std::vector<std::string> presets() const;
  bool notContains(const std::string& presetName) const noexcept;
  bool contains(const std::string& presetName) const noexcept;
  std::filesystem::path pathToExistingPreset(
      const std::string& presetName) const;
  std::filesystem::path pathToPreset(const std::string& presetName) const;

private:
  std::filesystem::path _presetsPath;
};
}  // namespace eden_vst
