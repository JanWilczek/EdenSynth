#pragma once
#include <string>
#include <filesystem>

namespace eden::plugin {
class Preset {
public:
  struct Args {
    std::string name;
    bool isFactory;
    std::filesystem::path absolutePath;
  };

  explicit Preset(Args&&);

  [[nodiscard]] const std::string& name() const noexcept { return _name; }

  [[nodiscard]] bool isFactory() const noexcept { return _isFactory; }

  [[nodiscard]] const std::filesystem::path& absolutePath() const noexcept {
    return _absolutePath;
  }

private:
  std::string _name;
  bool _isFactory;
  std::filesystem::path _absolutePath;
};
}  // namespace eden::plugin
