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

		/// <returns>unique id assigned to this oscillator</returns>
		OscillatorId getId() const noexcept;

		/// <summary>
		/// Sets the source (timbre, waveform) of this oscillator.
		/// </summary>
		/// <param name="source">oscillator source created by one of the <c>EdenSynthesiser</c>'s methods.</param>
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
