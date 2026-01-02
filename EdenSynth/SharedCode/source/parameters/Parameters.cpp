#include "parameters/Parameters.h"
#include "utility/EdenAssert.h"

namespace eden::plugin {
Parameters Parameters::from(juce::ValueTree vt) {
  juce::Array<juce::var> parameters{};

  for (const auto child : vt) {
    if (child.hasType("PARAM")) {
      if (child.hasProperty("id") && child.hasProperty("value")) {
        juce::DynamicObject::Ptr parameter{new juce::DynamicObject};
        parameter->setProperty("id", child["id"]);
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

void updateApvts(juce::AudioProcessorValueTreeState& state,
                 const Parameters& p) {
  const auto serializedParameters = p.toVar();
  const auto* const parameterArray =
      serializedParameters["parameters"].getArray();
  EDEN_ASSERT(parameterArray);
  for (const auto& element : *parameterArray) {
    const auto id = element["id"].toString();
    if (auto* parameter = state.getParameter(id)) {
      parameter->setValueNotifyingHost(
          parameter->convertTo0to1(float{element["value"]}));
    }
  }
}
}  // namespace eden::plugin
