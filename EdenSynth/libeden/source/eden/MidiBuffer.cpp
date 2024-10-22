///
/// \author Jan Wilczek
/// \date 02.09.2018
///
#include "eden/MidiBuffer.h"

namespace eden {
unsigned MidiBuffer::getNumEvents() const noexcept {
  return static_cast<unsigned>(_messages.size());
}

void MidiBuffer::addEvent(const MidiMessage& messageToAdd,
                          int messageSamplePosition) {
  _messages.push_back({messageToAdd, messageSamplePosition});
}

void MidiBuffer::clear() {
  _messages.clear();
}

MidiBuffer::MidiIterator MidiBuffer::begin() {
  return _messages.begin();
}

MidiBuffer::MidiIterator MidiBuffer::end() {
  return _messages.end();
}
}  // namespace eden
