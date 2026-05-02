#pragma once
#include <string>
#include <wolfsound/juce/wolfsound_SerializedParameters.hpp>

namespace eden::plugin {
using PresetId = std::string;

struct PresetMetadata {
  static constexpr auto currentPresetVersion = 1;

  std::string name;
  bool isFactory;
  PresetId id;
  int presetVersion = currentPresetVersion;
};

using ParameterValue = std::variant<float, int, bool, std::string>;

struct ParameterIdAndValue {
  std::string id;
  ParameterValue value;
};

using ParameterValues = std::vector<ParameterIdAndValue>;

struct PresetData {
  PresetMetadata metadata;
  ParameterValues parameters;
};

class PresetV2 {
public:
  PresetV2(PresetMetadata metadata, wolfsound::SerializedParameters parameters)
      : _metadata{std::move(metadata)}, _parameters{std::move(parameters)} {}
  [[nodiscard]] const wolfsound::SerializedParameters& parameters() const {
    return _parameters;
  }
  [[nodiscard]] const std::string& name() const { return _metadata.name; }
  [[nodiscard]] bool isFactory() const noexcept { return _metadata.isFactory; }
  [[nodiscard]] const PresetId& id() const noexcept { return _metadata.id; }
  [[nodiscard]] const PresetMetadata& metadata() const noexcept {
    return _metadata;
  }

private:
  PresetMetadata _metadata;
  wolfsound::SerializedParameters _parameters;
};
}  // namespace eden::plugin
