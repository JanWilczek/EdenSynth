#include "parameters/Parameters.h"

namespace eden::plugin {
Parameters Parameters::from(juce::ValueTree vt) {
  juce::Array<juce::var> parameters{};

  for (const auto child : vt) {
    if (child.hasType("PARAM")) {
      if (child.hasProperty("id") && child.hasProperty("value")) {
        juce::DynamicObject::Ptr parameter{new juce::DynamicObject};
        parameter->setProperty("name", child["id"]);
        parameter->setProperty("value", double{child["value"]});
        parameters.add({parameter});
      }
    }
  }

  DynamicObject::Ptr root{new juce::DynamicObject};
  root->setProperty("parameters", parameters);
  return Parameters{{root}};
}

Parameters Parameters::from(juce::var v) {
  return Parameters{std::move(v)};
}

void updateApvts(juce::AudioProcessorValueTreeState&, const Parameters&) {}
}  // namespace eden::plugin
