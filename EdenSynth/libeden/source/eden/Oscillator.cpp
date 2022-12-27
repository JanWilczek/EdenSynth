///
/// \author Jan Wilczek
/// \date 12.11.2018
///
#include "eden/Oscillator.h"
#include "eden_impl/OscillatorImpl.h"

namespace eden {
Oscillator::Oscillator(std::unique_ptr<OscillatorImpl> impl)
    : _impl(std::move(impl)) {}

OscillatorId Oscillator::getId() const noexcept {
  return _impl->getId();
}

void Oscillator::setSource(std::unique_ptr<OscillatorSource> source) {
  _impl->setSource(std::move(source));
}

void Oscillator::setOctaveTransposition(int octaveShift) {
  _impl->setOctaveTransposition(octaveShift);
}

void Oscillator::setSemitoneTransposition(int semitoneShift) {
  _impl->setSemitoneTransposition(semitoneShift);
}

void Oscillator::setCentTransposition(int centShift) {
  _impl->setCentTransposition(centShift);
}

void Oscillator::setVolume(float volume) {
  _impl->setVolume(volume);
}

void Oscillator::setOn(bool isOn) {
  _impl->setOn(isOn);
}
}  // namespace eden
