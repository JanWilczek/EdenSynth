#include "Presets.h"
#include <algorithm>
#include <filesystem>
#include <utility/EdenAssert.h>
#include <JuceHeader.h>

// NEW PRESETS

namespace eden::plugin {
namespace {
std::string presetNameFrom(const std::filesystem::path& presetPath) {
  return presetPath.filename().replace_extension("").string();
}

Preset presetFrom(const std::filesystem::path& presetPath, bool isFactory) {
  return Preset{Preset::Args{
      .name = presetNameFrom(presetPath),
      .isFactory = isFactory,
      .absolutePath = presetPath,
  }};
}
}  // namespace

Presets::Presets(std::filesystem::path systemPresetsPath,
                 std::filesystem::path userPresetsPath)
    : _systemPresetsPath{std::move(systemPresetsPath)},
      _userPresetsPath{std::move(userPresetsPath)} {}

auto Presets::presets() -> const Container& {
  using namespace std::filesystem;

  if (_presets.empty()) {
    scanForPresets();
  }

  return _presets;
}

bool Presets::containsUserPreset(const std::string& presetName) {
  return std::ranges::find_if(_presets, [&](const auto& preset) {
           return !preset.isFactory() && preset.name() == presetName;
         }) != _presets.end();
}

const Preset& Presets::createPreset(const std::string& name) {
  _presets.emplace_back(Preset::Args{
      .name = name,
      .isFactory = false,
      .absolutePath = _userPresetsPath / name,
  });
  return _presets.back();
}

void Presets::scanForPresets() {
  using namespace std::filesystem;

  Container result;

  auto scanDirectory = [&](const auto& directoryPath, bool isFactory) {
    if (exists(directoryPath) && is_directory(directoryPath)) {
      std::transform(directory_iterator{directoryPath}, directory_iterator{},
                     std::back_inserter(result), [=](const auto& path) {
                       return presetFrom(path, isFactory);
                     });
    }
  };

  scanDirectory(_systemPresetsPath, true);
  scanDirectory(_userPresetsPath, false);

  std::swap(_presets, result);
}
}  // namespace eden::plugin
