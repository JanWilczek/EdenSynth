#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  explicit Parameters(juce::ValueTree vt) : _valueTree{std::move(vt)} {}

  juce::var toVar() const {
    DynamicObject::Ptr root{new juce::DynamicObject};
    root->setProperty("parameters", juce::Array<juce::var>{});
    return {root};
  }

private:
  juce::ValueTree _valueTree;
};
}  // namespace eden::plugin
