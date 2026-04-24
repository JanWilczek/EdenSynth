#pragma once
#include <optional>
#include <JuceHeader.h>

namespace eden::plugin {
/** @brief Wrapper around a properly structured juce::var containing parameter
   IDs and their values. Use it for storing parameter values.

    Conceptually, this class holds a JSON object structured as follows
    @code
    {
      "parameters": [
        {
          "id": "floatParam",
          "value": 1.0
        },
        {
          "id": "boolParam",
          "value": false
        },
        {
          "id": "intParam",
          "value": 5
        },
        {
          "id": "choiceParam",
          "value": "choice 0"
        }
      ]
    }
    @endcode
*/
class SerializedParameters {
public:
  /** @brief Return an object of this class if the input is a
     juce::DynamicObject with a "parameters" field (see class description),
     std::nullopt otherwise */
  [[nodiscard]] static std::optional<SerializedParameters> from(
      const juce::var&);

  /** @brief Return an object of this class if the input is an array filled with
     juce::DynamicObject objects, each having an "id" field and a "value" field,
     std::nullopt otherwise */
  [[nodiscard]] static std::optional<SerializedParameters> from(
      const juce::Array<juce::var>&);

  /** @brief Return a juce::DynamicObject with the structure as shown in the
   * class description */
  [[nodiscard]] const juce::var& toVar() const noexcept { return _impl; }

  /** @brief Return the value of the "parameters" field with the structure as
   * shown in the class description */
  [[nodiscard]] const juce::Array<juce::var>& toVarArray() const noexcept {
    return *_impl.getProperty("parameters", juce::var{juce::Array<juce::var>{}})
                .getArray();
  }

private:
  [[nodiscard]] static SerializedParameters fromCorrect(
      const juce::Array<juce::var>&);

  explicit SerializedParameters(juce::var v) : _impl{std::move(v)} {}

  juce::var _impl;
};
}  // namespace eden::plugin
