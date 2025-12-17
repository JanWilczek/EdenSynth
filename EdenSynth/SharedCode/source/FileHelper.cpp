#include "FileHelper.h"
#include <filesystem>
#include "JuceHeader.h"

namespace eden::plugin {
namespace {
constexpr auto presetsFolderName = "presets";

void createDirectoryIfNotExists(const std::filesystem::path& directory) {
  if (!std::filesystem::exists(directory)) {
    [[maybe_unused]] const auto creationResult =
        std::filesystem::create_directories(directory);
    jassert(creationResult);
  }
}

std::filesystem::path pluginSubfolderIn(
    const juce::File& systemOrUserDataFolder) {
  return systemOrUserDataFolder.getChildFile(JucePlugin_Manufacturer)
      .getChildFile(JucePlugin_Name)
      .getFullPathName()
      .toStdString();
}

std::filesystem::path userDataPath() {
  static const auto directory =
      pluginSubfolderIn(juce::File::getSpecialLocation(
          juce::File::SpecialLocationType::userDocumentsDirectory));

  createDirectoryIfNotExists(directory);

  return directory;
}
}  // namespace

std::filesystem::path FileHelper::assetsPath() {
  static const auto pluginDirectory =
      pluginSubfolderIn(juce::File::getSpecialLocation(
          juce::File::SpecialLocationType::commonApplicationDataDirectory));
  jassert(std::filesystem::exists(pluginDirectory));
  return pluginDirectory;
}

std::filesystem::path FileHelper::systemPresetsPath() {
  static const auto result = assetsPath() / presetsFolderName;

  jassert(std::filesystem::exists(result));
  jassert(std::filesystem::is_directory(result));

  return result;
}

std::filesystem::path FileHelper::userPresetsPath() {
  static const auto result = userDataPath() / presetsFolderName;

  createDirectoryIfNotExists(result);

  return result;
}
}  // namespace eden::plugin
