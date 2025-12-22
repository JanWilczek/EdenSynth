#pragma once
#include <JuceHeader.h>

namespace eden::plugin {
class Parameters {
public:
  explicit Parameters(juce::ValueTree vt) : _valueTree{std::move(vt)} {}

  juce::var toVar() const {
    juce::Array<juce::var> parameters{};

    for (const auto child : _valueTree) {
      if (child.hasType("PARAM")) {
        if (child.hasProperty("id") && child.hasProperty("value")) {
          juce::DynamicObject::Ptr parameter{new juce::DynamicObject};
          parameter->setProperty("name", child["id"]);
          parameter->setProperty("value", child["value"]);
          parameters.add({parameter});
        }
      }
    }

    DynamicObject::Ptr root{new juce::DynamicObject};
    root->setProperty("parameters", parameters);
    return {root};
  }

private:
  juce::ValueTree _valueTree;
};
}  // namespace eden::plugin
