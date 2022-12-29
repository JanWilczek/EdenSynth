#pragma once
#include <filesystem>
#include "JuceHeader.h"

namespace eden_vst {
class FileHelper {
public:
  static std::filesystem::path assetsPath() {
    return std::filesystem::path(
               File::getSpecialLocation(
                   File::SpecialLocationType::currentExecutableFile)
                   .getParentDirectory()
                   .getFullPathName()
                   .toStdString()) /
           "assets";
  }
};
}  // namespace eden_vst
