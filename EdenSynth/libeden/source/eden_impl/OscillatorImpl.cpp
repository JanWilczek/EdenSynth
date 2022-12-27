///
/// \author Jan Wilczek
/// \date 11.11.2018
///
#include "eden_impl/OscillatorImpl.h"
#include <stdexcept>
#include "eden/OscillatorSource.h"
#include "settings/Settings.h"

namespace eden {
OscillatorImpl::OscillatorImpl(settings::Settings& settings,
                               std::unique_ptr<OscillatorSource> source)
    : _ext_settings(settings),
      _id(_ext_settings.addOscillator(source->getId())),
      _oscillatorSource(std::move(source)),
      _octaveShift(0),
      _semitoneShift(0),
      _centShift(0),
      _volume(1.f),
      _isOn(true) {}

OscillatorImpl::~OscillatorImpl() {
  _ext_settings.removeOscillator(getId());
}

OscillatorId OscillatorImpl::getId() const noexcept {
  return _id;
}

void OscillatorImpl::setSource(std::unique_ptr<OscillatorSource> source) {
  if (source->getId() != _oscillatorSource->getId()) {
    _oscillatorSource = std::move(source);
    _ext_settings.setOscillatorSource(getId(), _oscillatorSource->getId());
  }
}

void OscillatorImpl::setOctaveTransposition(int octaveShift) {
  if (octaveShift != _octaveShift) {
    _octaveShift = octaveShift;
    _ext_settings.setOctaveTransposition(getId(), _octaveShift);
  }
}

void OscillatorImpl::setSemitoneTransposition(int semitoneShift) {
  if (semitoneShift != _semitoneShift) {
    _semitoneShift = semitoneShift;
    _ext_settings.setSemitoneTransposition(getId(), _semitoneShift);
  }
}

void OscillatorImpl::setCentTransposition(int centShift) {
  if (centShift != _centShift) {
    _centShift = centShift;
    _ext_settings.setCentTransposition(getId(), _centShift);
  }
}

void OscillatorImpl::setVolume(float volume) {
  if (volume < 0.f || volume > 1.f) {
    throw std::invalid_argument(
        "Oscillator's volume should be in range [0; 1].");
  }

  if (volume != _volume) {
    _volume = volume;
    _ext_settings.setOscillatorVolume(getId(), volume);
  }
}

void OscillatorImpl::setOn(bool isOn) {
  if (isOn != _isOn) {
    _isOn = isOn;
    _ext_settings.setOscillatorOn(getId(), _isOn);
  }
}
}  // namespace eden
