#include "parameters/Parameters.h"
#include "utility/EdenAssert.h"

namespace eden::plugin {
Parameters Parameters::from(juce::ValueTree vt) {
  juce::Array<juce::var> parameters{};

  for (const auto child : vt) {
    if (child.hasType("PARAM")) {
      if (child.hasProperty("id") && child.hasProperty("value")) {
        const juce::DynamicObject::Ptr parameter{new juce::DynamicObject};
        parameter->setProperty("id", child["id"]);
        parameter->setProperty("value", double{child["value"]});
        parameters.add({parameter});
      }
    }
  }

  return Parameters::from(parameters);
}

Parameters Parameters::from(const juce::var& v) {
  EDEN_ASSERT(v.hasProperty("parameters") && v["parameters"].isArray());
  return Parameters{v};
}

Parameters Parameters::from(const juce::Array<juce::var>& parameterArray) {
  const DynamicObject::Ptr root{new juce::DynamicObject};
  root->setProperty("parameters", parameterArray);
  return Parameters{{root}};
}

std::optional<Parameters> Parameters::fromChecked(const juce::var& v) {
  if (v.hasProperty("parameters") && v["parameters"].isArray()) {
    return Parameters::fromChecked(*v["parameters"].getArray());
  }

  return {};
}

std::optional<Parameters> Parameters::fromChecked(
    const juce::Array<juce::var>& parameterArray) {
  auto hasIdAndValue = [](const auto& var) {
    return var.hasProperty("id") && var["id"].isString() &&
           var.hasProperty("value");
  };
  if (std::ranges::all_of(parameterArray, hasIdAndValue)) {
    return Parameters::from(parameterArray);
  }

  return {};
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
