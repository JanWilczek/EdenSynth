#include "parameters/Parameters.h"
#include "utility/EdenAssert.h"

namespace eden::plugin {
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
}  // namespace eden::plugin
