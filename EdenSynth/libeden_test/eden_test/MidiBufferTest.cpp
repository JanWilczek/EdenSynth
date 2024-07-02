///
/// \author Jan Wilczek
/// \date 07.10.2018
///

#include "eden/MidiBuffer.h"
#include "eden/MidiMessage.h"

namespace libeden_test {
TEST(MidiBufferTest, SingleMessage) {
  const eden::MidiMessage message(
      0, eden::MidiMessage::MidiMessageType::Aftertouch);
  eden::MidiBuffer buffer;

  buffer.addEvent(message, 2);

  ASSERT_EQ(buffer.getNumEvents(), 1);
  ASSERT_EQ(buffer.begin()->message.getType(),
            eden::MidiMessage::MidiMessageType::Aftertouch);
}

TEST(MidiBufferTest, MultipleNoteOnMessages) {
  eden::MidiMessage message(0, eden::MidiMessage::MidiMessageType::NoteOn);
  message.setNoteNumber(66);
  message.setVelocity(0.5f);

  eden::MidiBuffer buffer;

  for (int i = 0; i < 5; ++i) {
    buffer.addEvent(message, i * 10);
  }

  ASSERT_EQ(buffer.getNumEvents(), 5);

  auto iterator = buffer.begin();

  auto messageCheck = [&](int timeStamp) {
    EXPECT_EQ(iterator->message.getType(),
              eden::MidiMessage::MidiMessageType::NoteOn);
    EXPECT_FLOAT_EQ(iterator->message.getVelocity(), 0.5f);
    EXPECT_EQ(iterator->message.getChannel(), 0);
    EXPECT_EQ(iterator->sampleTimeStamp, timeStamp);
  };

  messageCheck(0);
  ++iterator;
  messageCheck(10);
  ++iterator;
  messageCheck(20);
  ++iterator;
  messageCheck(30);
  ++iterator;
  messageCheck(40);
  ++iterator;

  EXPECT_EQ(iterator, buffer.end());

  ASSERT_NO_THROW(buffer.clear());
}
}  // namespace libeden_test