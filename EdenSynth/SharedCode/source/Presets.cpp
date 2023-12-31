#include "Presets.h"

namespace {
std::string presetNameFrom(const std::filesystem::path& presetPath) {
  return presetPath.filename().replace_extension("").string();
}
}  // namespace

namespace eden_vst {
Presets::Presets(std::filesystem::path presetsPath)
    : _presetsPath(std::move(presetsPath)) {}

std::vector<std::string> Presets::presets() const {
  using namespace std::filesystem;
  std::vector<std::string> result;
  std::transform(directory_iterator{_presetsPath}, directory_iterator{},
                 std::back_inserter(result),
                 [](const path& path) { return presetNameFrom(path); });
  return result;
}

bool Presets::notContains(const std::string& presetName) const noexcept {
  const auto presetNames = presets();
  return std::ranges::find(presetNames, presetName) == presetNames.end();
}

bool Presets::contains(const std::string& presetName) const noexcept {
  return not notContains(presetName);
}

std::filesystem::path Presets::pathToExistingPreset(
    const std::string& presetName) const {
  using namespace std::filesystem;
  const auto presetIterator = std::find_if(
      directory_iterator{_presetsPath}, directory_iterator{},
      [&presetName](const path& p) { return presetNameFrom(p) == presetName; });
  if (presetIterator == directory_iterator{}) {
    throw std::runtime_error("Non-existent preset name!");
  }

  return *presetIterator;
}

std::filesystem::path Presets::pathToPreset(
    const std::string& presetName) const {
  return (_presetsPath / presetName).replace_extension("xml");
}
}  // namespace eden_vst
