#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include "eden/SampleType.h"
#include "eden/OscillatorParameters.h"
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable
{
	class SynthOscillator
	{
	public:
		SynthOscillator() = default;
		SynthOscillator(std::shared_ptr<IOscillatorSource> oscillatorSource);

		OscillatorId getId();
		SampleType getSample();
		void setSource(std::shared_ptr<IOscillatorSource> oscillatorSource);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);

		void setPitch(float pitch);

		void setSampleRate(float sampleRate);

		float pitch();

	private:
		void setOscillatorPitch();

		std::shared_ptr<IOscillatorSource> _oscillatorSource;
		OscillatorId _id;
		int _octaveShift;
		int _semitoneShift;
		int _centShift;
		float _originalPitch;
	};
}
