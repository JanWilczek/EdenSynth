#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.11.2018
/// 
#include <memory>
#include "eden/OscillatorParameters.h"

namespace eden
{
	class OscillatorImpl;
	class OscillatorSource;

	class Oscillator
	{
	public:
		Oscillator(std::unique_ptr<OscillatorImpl> impl);

		void setSource(std::unique_ptr<OscillatorSource> source);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);

		OscillatorId getId();

	private:
		std::unique_ptr<OscillatorImpl> _impl;
	};
}
