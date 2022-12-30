#include "Presets.h"

namespace eden_vst {
Presets::Presets(std::filesystem::path presetsPath)
    : _presetsPath(std::move(presetsPath)) {}

std::vector<std::string> Presets::presets() const {
  using namespace std::filesystem;
  std::vector<std::string> result;
  std::transform(directory_iterator{_presetsPath}, directory_iterator{},
                 std::back_inserter(result), [](const path& path) {
                   return path.filename().replace_extension("").string();
                 });
  return result;
}
}  // namespace eden_vst
