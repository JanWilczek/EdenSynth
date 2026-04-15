#pragma once
#include <expected>

namespace eden::plugin {
enum class PresetLoadingSuccess { Ok };
enum class PresetLoadingError { DoesNotExist, FailedToReadFile, NoPermission };

using PresetLoadingResult =
    std::expected<PresetLoadingSuccess, PresetLoadingError>;
}  // namespace eden::plugin
