#pragma once
#include <expected>

namespace eden_vst {
enum class PresetLoadingSuccess { Ok };
enum class PresetLoadingError { DoesNotExist, WrongTag };

using PresetLoadingResult =
    std::expected<PresetLoadingSuccess, PresetLoadingError>;
}  // namespace eden_vst
