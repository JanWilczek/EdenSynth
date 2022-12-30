#pragma once
#include <filesystem>
#include <vector>

namespace eden_vst {
class Presets {
public:
  explicit Presets(std::filesystem::path presetsPath);
  std::vector<std::string> presets() const;

private:
  std::filesystem::path _presetsPath;
};
}  // namespace eden_vst
