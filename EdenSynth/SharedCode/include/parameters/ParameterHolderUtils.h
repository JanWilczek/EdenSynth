#pragma once
#include "presets/Preset.h"
#include <wolfsound/juce/wolfsound_ParameterHolder.hpp>

namespace eden::plugin {
class UpdatingParameterVisitor : public wolfsound::JuceParameterVisitor {
  [[nodiscard]] decltype(auto) findParameterWithId(
      const std::string& id) const noexcept {
    return std::ranges::find_if(
        _parameters, [&](const ParameterIdAndValue& p) { return p.id == id; });
  }

public:
  explicit UpdatingParameterVisitor(const ParameterValues& parameters)
      : _parameters{parameters} {}

  void visit(AudioParameterBool& p) override { visitImpl<bool>(p); }

  void visit(AudioParameterFloat& p) override { visitImpl<float>(p); }

  void visit(AudioParameterInt& p) override { visitImpl<int>(p); }

  void visit(AudioParameterChoice& parameter) override {
    const auto it =
        findParameterWithId(parameter.getParameterID().toStdString());
    if (it != _parameters.end()) {
      if (const auto* const choiceName = get_if<std::string>(&(it->value))) {
        const auto choiceIndex = parameter.choices.indexOf(*choiceName);
        // don't "clamp" incorrect values, ignore them
        if (0 <= choiceIndex && choiceIndex < parameter.choices.size()) {
          parameter = choiceIndex;
        }
      }
    }
  }

private:
  template <typename T>
  void visitImpl(auto& parameter) const {
    const auto it =
        findParameterWithId(parameter.getParameterID().toStdString());
    if (it != _parameters.end()) {
      if (const auto value = get_if<T>(&(it->value))) {
        parameter = *value;
      }
    }
  }

  const ParameterValues& _parameters;

  JUCE_DECLARE_NON_MOVEABLE(UpdatingParameterVisitor)
};

inline void update(wolfsound::JuceParameterHolder& ph,
                   const ParameterValues& parameters) {
  UpdatingParameterVisitor visitor{parameters};
  ph.accept(visitor);
}

class ParameterValuesExtractor : public wolfsound::JuceParameterVisitor {
public:
  ParameterValuesExtractor() = default;

  void visit(juce::AudioParameterFloat& parameter) override {
    visitImpl(parameter, parameter.get());
  }

  void visit(juce::AudioParameterBool& parameter) override {
    visitImpl(parameter, parameter.get());
  }

  void visit(juce::AudioParameterInt& parameter) override {
    visitImpl(parameter, parameter.get());
  }

  void visit(juce::AudioParameterChoice& parameter) override {
    visitImpl(parameter, parameter.getCurrentChoiceName().toStdString());
  }

  [[nodiscard]] ParameterValues result() const { return _result; }

private:
  template <class P, class V>
  void visitImpl(const P& parameter, V&& value) {
    _result.emplace_back(parameter.getParameterID().toStdString(),
                         std::forward<V>(value));
  }

  ParameterValues _result;

  JUCE_DECLARE_NON_MOVEABLE(ParameterValuesExtractor)
};

inline ParameterValues parameterIdsAndValues(
    wolfsound::JuceParameterHolder& ph) {
  ParameterValuesExtractor visitor;
  ph.accept(visitor);
  return visitor.result();
}
}  // namespace eden::plugin