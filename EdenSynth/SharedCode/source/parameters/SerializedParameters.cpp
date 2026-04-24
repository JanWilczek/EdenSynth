#include "parameters/SerializedParameters.h"

namespace eden::plugin {
SerializedParameters SerializedParameters::fromCorrect(
    const juce::Array<juce::var>& parameterArray) {
  const DynamicObject::Ptr root{new juce::DynamicObject};
  root->setProperty("parameters", parameterArray);
  return SerializedParameters{{root}};
}

std::optional<SerializedParameters> SerializedParameters::fromChecked(
    const juce::var& v) {
  if (v.hasProperty("parameters") && v["parameters"].isArray()) {
    return SerializedParameters::fromChecked(*v["parameters"].getArray());
  }

  return {};
}

std::optional<SerializedParameters> SerializedParameters::fromChecked(
    const juce::Array<juce::var>& parameterArray) {
  auto hasIdAndValue = [](const auto& var) {
    return var.hasProperty("id") && var["id"].isString() &&
           var.hasProperty("value");
  };
  if (std::ranges::all_of(parameterArray, hasIdAndValue)) {
    return SerializedParameters::fromCorrect(parameterArray);
  }

  return {};
}
}  // namespace eden::plugin
