#pragma once
#include <filesystem>

namespace eden_vst {
class FileHelper {
public:
  static std::filesystem::path assetsPath();
  static std::filesystem::path presetsPath();
};
}  // namespace eden_vst
