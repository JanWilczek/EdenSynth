#pragma once
#include <expected>

namespace eden_vst {
enum class PresetSavingSuccess { Ok };
enum class PresetSavingError {
  PresetWithNameExists,
  FailedToCreatePresetFile,
  FailedToWritePresetFile,
  InvalidPresetName
};

using PresetSavingResult =
    std::expected<PresetSavingSuccess, PresetSavingError>;
}  // namespace eden_vst
