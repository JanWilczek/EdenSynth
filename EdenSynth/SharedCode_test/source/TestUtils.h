#pragma once
#include <filesystem>

namespace eden::plugin {
[[nodiscard]] const std::filesystem::path& testAssetsPath();
}