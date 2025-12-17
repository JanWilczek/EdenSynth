#pragma once
#include <filesystem>

namespace eden_vst {
class FileHelper {
public:
  static std::filesystem::path assetsPath();

  /// <summary>
  /// Path to the folder with system-wide presets. This path is treated
  /// as read-only.
  /// </summary>
  static std::filesystem::path systemPresetsPath();

  /// <summary>
  /// Path to the folder with user presets. This is the path where new
  /// presets will be saved.
  /// </summary>
  static std::filesystem::path userPresetsPath();
};
}  // namespace eden_vst
