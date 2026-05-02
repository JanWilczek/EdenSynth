#pragma once
#include <string>
#include <wolfsound/juce/wolfsound_SerializedParameters.hpp>
#include <JuceHeader.h>

namespace eden::plugin {
using PresetId = std::string;

struct PresetMetadata {
  static constexpr auto currentPresetVersion = 1;

  std::string name;
  bool isFactory = false;
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
}  // namespace eden::plugin

// serialization
template <>
struct juce::SerialisationTraits<eden::plugin::PresetMetadata> {
  static constexpr auto marshallingVersion =
      eden::plugin::PresetMetadata::currentPresetVersion;

  template <class Archive, class T>
  static void serialise(Archive& archive, T& metadata) {
    if (!archive.getVersion().has_value()) {
      // fail parsing (a workaround since there's no archive.fail() function)
      auto placeholder = 0;
      archive(named("__version__", placeholder));
      return;
    }
    if constexpr (!std::is_const_v<T>) {
      metadata.presetVersion = archive.getVersion().value();
    }
    archive(named("id", metadata.id), named("name", metadata.name));
  }
};

template <>
struct juce::SerialisationTraits<eden::plugin::ParameterValue> {
  static constexpr auto marshallingVersion = std::nullopt;

  template <class Archive>
  static void save(Archive& archive, const eden::plugin::ParameterValue& t) {
    std::visit([&archive](const auto& value) { archive(value); }, t);
  }

  template <class Archive>
  static void load(Archive& archive, eden::plugin::ParameterValue& t) {
    juce::var v;
    archive(v);
    if (v.isString()) {
      t = v.toString().toStdString();
    } else if (v.isInt() || v.isInt64()) {
      t = static_cast<int>(v);
    } else if (v.isDouble()) {
      t = static_cast<float>(v);
    } else if (v.isBool()) {
      t = static_cast<bool>(v);
    }
  }
};

template <>
struct juce::SerialisationTraits<eden::plugin::ParameterIdAndValue> {
  static constexpr auto marshallingVersion = std::nullopt;

  template <class Archive, class T>
  static void serialise(Archive& archive, T& t) {
    archive(named("id", t.id), named("value", t.value));
  }
};

template <>
struct juce::SerialisationTraits<eden::plugin::PresetData> {
  // PresetMetadata handles preset version
  static constexpr auto marshallingVersion = std::nullopt;

  template <class Archive, class T>
  static void serialise(Archive& archive, T& t) {
    archive(t.metadata);
    archive(named("parameters", t.parameters));
  }
};

namespace eden::plugin {
/** @brief Class representing an immutable preset */
class PresetV2 {
public:
  explicit PresetV2(PresetData data)
      : _metadata{std::move(data.metadata)},
        _parameters{std::move(data.parameters)} {}

  PresetV2(PresetMetadata metadata, ParameterValues parameters)
      : _metadata{std::move(metadata)}, _parameters{std::move(parameters)} {}

  PresetV2(PresetMetadata metadata, wolfsound::SerializedParameters parameters)
      : _metadata{std::move(metadata)},
        _parameters{
            juce::FromVar::convert<ParameterValues>(parameters.toVarArray())
                .value()} {}

  [[nodiscard]] wolfsound::SerializedParameters parameters() const {
    return wolfsound::SerializedParameters::from(
               *juce::ToVar::convert(_parameters).value().getArray())
        .value();
  }
  [[nodiscard]] const std::string& name() const { return _metadata.name; }
  [[nodiscard]] bool isFactory() const noexcept { return _metadata.isFactory; }
  [[nodiscard]] const PresetId& id() const noexcept { return _metadata.id; }
  [[nodiscard]] const PresetMetadata& metadata() const noexcept {
    return _metadata;
  }

private:
  PresetMetadata _metadata;
  ParameterValues _parameters;
};
}  // namespace eden::plugin
