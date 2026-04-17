#include "TestUtils.h"

namespace eden::plugin {
const std::filesystem::path& testAssetsPath() {
  static const auto result =
      std::filesystem::path(__FILE__).parent_path().parent_path() / "assets";
  return result;
}
}  // namespace eden::plugin