#include <parameters/Parameters.h>
#include <gtest/gtest.h>
#include <memory>
#include <juce_core/juce_core.h>

namespace eden::plugin {
TEST(Parameters, FromVarChecked) {
  const auto result = Parameters::fromChecked(juce::JSON::parse(R"-(
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

TEST(Parameters, FromVarArrayChecked) {
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

  const auto result = Parameters::fromChecked(*array.getArray());

  EXPECT_TRUE(result.has_value());
}
}  // namespace eden::plugin
