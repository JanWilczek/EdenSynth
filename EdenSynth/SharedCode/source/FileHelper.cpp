#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
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

std::filesystem::path FileHelper::presetsPath() {
  static const auto presetsPath = assetsPath() / "presets";

  jassert(std::filesystem::exists(presetsPath));

  return presetsPath;
}
}  // namespace eden_vst
