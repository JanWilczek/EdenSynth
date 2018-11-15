#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.11.2018
/// 
#include <memory>
#include "eden/OscillatorParameters.h"
#include "eden_impl/OscillatorImpl.h"

namespace eden
{
	class OscillatorSource;

	class Oscillator
	{
	public:
		Oscillator(std::unique_ptr<OscillatorImpl> impl);

		OscillatorId getId() const noexcept;
		void setSource(std::unique_ptr<OscillatorSource> source);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);
		void setVolume(float volume);
		void setOn(bool isOn);

	private:
		std::unique_ptr<OscillatorImpl> _impl;
	};
}
