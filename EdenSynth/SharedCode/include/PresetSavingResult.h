#pragma once
#include <expected>

namespace eden::plugin {
enum class PresetSavingSuccess { Ok };
enum class PresetSavingError {
  PresetWithNameExists,
  FailedToCreatePresetFile,
  FailedToWritePresetFile,
  InvalidPresetName
};

using PresetSavingResult =
    std::expected<PresetSavingSuccess, PresetSavingError>;
}  // namespace eden::plugin
