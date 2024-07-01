#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
std::filesystem::path FileHelper::assetsPath() {
  const auto pluginDirectory = std::filesystem::path(
      File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile)
          .getParentDirectory()
          .getFullPathName()
          .toStdString());
  constexpr auto ASSETS_DIRNAME = "assets";

  // Is the assets folder in the folder with the executable?
  if (const auto assetsDirectory = (pluginDirectory / ASSETS_DIRNAME);
      std::filesystem::exists(assetsDirectory)) {
    return assetsDirectory;
  }

  // Is the assets folder in the parent folder of the executable?
  if (const auto assetsDirectory =
          (pluginDirectory.parent_path() / ASSETS_DIRNAME);
      std::filesystem::exists(assetsDirectory)) {
    return assetsDirectory;
  }

  // Create the assets folder.
  const auto assetsDirectory = (pluginDirectory / ASSETS_DIRNAME);
  std::filesystem::create_directory(assetsDirectory);
  return assetsDirectory;
}

std::filesystem::path FileHelper::presetsPath() {
  const auto presetsPath = assetsPath() / "presets";

  if (not std::filesystem::exists(presetsPath)) {
    std::filesystem::create_directory(presetsPath);
  }

  return presetsPath;
}
}  // namespace eden_vst
