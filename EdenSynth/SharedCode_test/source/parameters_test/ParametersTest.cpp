#include "parameters/SerializedParameters.h"
#include <gtest/gtest.h>
#include <memory>
#include <juce_core/juce_core.h>

namespace eden::plugin {
TEST(SerializedParameters, FromVarChecked) {
  const auto result = SerializedParameters::fromChecked(juce::JSON::parse(R"-(
{
  "name": "Min (Factory Preset)",
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
)-"));

  EXPECT_TRUE(result.has_value());
}

TEST(SerializedParameters, FromVarArrayChecked) {
  const auto array = juce::JSON::parse(R"-(
[
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
)-");
  ASSERT_TRUE(array.isArray());

  const auto result = SerializedParameters::fromChecked(*array.getArray());

  EXPECT_TRUE(result.has_value());
}
}  // namespace eden::plugin
