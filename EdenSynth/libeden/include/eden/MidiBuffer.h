#pragma once
///
/// \author Jan Wilczek
/// \date 27.08.2018
///
#include <vector>
#include "eden/MidiMessage.h"

namespace eden {
/// <summary>
/// Container of MIDI messages to be processed by the synthesiser.
/// </summary>
class MidiBuffer {
 public:
  /// <summary>
  /// Helper struct associating MIDI message with a sample time stamp.
  /// </summary>
  struct MidiStampedMessage {
    MidiMessage message;
    int sampleTimeStamp;
  };

  /// <summary>
  /// Iterator to iterate through MIDI messages.
  /// </summary>
  typedef std::vector<MidiStampedMessage>::iterator MidiIterator;

  MidiBuffer() = default;

  /// <returns>number of messages currently stored in the buffer</returns>
  unsigned int getNumEvents() const noexcept;

  /// <summary>
  /// Adds a MIDI message at a particular sample index in the audio buffer.
  /// </summary>
  /// <param name="messageToAdd"></param>
  /// <param name="messageSamplePosition"></param>
  void addEvent(const MidiMessage& messageToAdd, int messageSamplePosition);

  /// <summary>
  /// Removes all stored MIDI messages.
  /// </summary>
  void clear();

  /// <returns>iterator pointing at the beginning of the buffer</returns>
  MidiIterator begin();

  /// <returns>iterator pointing at the end of the buffer</returns>
  MidiIterator end();

 private:
  std::vector<MidiStampedMessage> _messages;
};
}  // namespace eden
