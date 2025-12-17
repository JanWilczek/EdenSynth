#include "FileHelper.h"
#include <filesystem>
#include "JuceHeader.h"

namespace eden_vst {
namespace {
constexpr auto presetsFolderName = "presets";

void createDirectoryIfNotExists(const std::filesystem::path& directory) {
  if (!std::filesystem::exists(directory)) {
    [[maybe_unused]] const auto creationResult =
        std::filesystem::create_directories(directory);
    jassert(creationResult);
  }
}

std::filesystem::path userDataPath() {
  static const auto directory = std::filesystem::path(
      File::getSpecialLocation(
          File::SpecialLocationType::userDocumentsDirectory)
          .getChildFile(JucePlugin_Manufacturer)
          .getChildFile(JucePlugin_Name)
          .getFullPathName()
          .toStdString());

  createDirectoryIfNotExists(directory);

  return directory;
}
}  // namespace

std::filesystem::path FileHelper::assetsPath() {
  static const auto pluginDirectory = std::filesystem::path(
      File::getSpecialLocation(
          File::SpecialLocationType::commonApplicationDataDirectory)
          .getChildFile(JucePlugin_Manufacturer)
          .getChildFile(JucePlugin_Name)
          .getFullPathName()
          .toStdString());
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
}  // namespace eden_vst
