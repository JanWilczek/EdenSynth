#pragma once
#include <expected>

namespace eden_vst {
enum class PresetLoadingSuccess { Ok };
enum class PresetLoadingError { DoesNotExist, FailedToReadFile, NoPermission };

using PresetLoadingResult =
    std::expected<PresetLoadingSuccess, PresetLoadingError>;
}  // namespace eden_vst
