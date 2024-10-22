#pragma once
///
/// \author Jan Wilczek
/// \date 11.11.2018
///
#include <memory>
#include "eden/OscillatorParameters.h"
#include "eden/OscillatorSource.h"

namespace eden {
namespace settings {
class Settings;
}

class OscillatorImpl {
public:
  OscillatorImpl(settings::Settings& settings,
                 std::unique_ptr<OscillatorSource> source);
  ~OscillatorImpl();

  OscillatorId getId() const noexcept;

  void setSource(std::unique_ptr<OscillatorSource> source);

  void setOctaveTransposition(int octaveShift);
  void setSemitoneTransposition(int semitoneShift);
  void setCentTransposition(int centShift);
  void setVolume(float volume) noexcept(false);
  void setOn(bool isOn);

private:
  settings::Settings& _ext_settings;
  OscillatorId _id;
  std::unique_ptr<OscillatorSource> _oscillatorSource;
  int _octaveShift;
  int _semitoneShift;
  int _centShift;
  float _volume;
  bool _isOn;
};
}  // namespace eden