#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
std::filesystem::path FileHelper::assetsPath() {
  return std::filesystem::path(
             File::getSpecialLocation(
                 File::SpecialLocationType::currentExecutableFile)
                 .getParentDirectory()
                 .getFullPathName()
                 .toStdString()) /
         "assets";
}

std::filesystem::path FileHelper::presetsPath() {
  return assetsPath() / "presets";
}
}  // namespace eden_vst
