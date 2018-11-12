#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>

namespace eden::synth::wavetable
{
	class SynthOscillatorSource;

	class SynthOscillator
	{
	public:


		void setSource(std::shared_ptr<SynthOscillatorSource> source);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);

		void setPitch(float pitch);
	};
}
