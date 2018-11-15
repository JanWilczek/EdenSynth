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
		SynthOscillator(OscillatorId id, std::unique_ptr<IOscillatorSource> oscillatorSource);
		SynthOscillator(const SynthOscillator&);
		SynthOscillator& operator=(const SynthOscillator&);

		OscillatorId getId() const noexcept;
		SampleType getSample();
		void reset();
		
		void setSource(std::unique_ptr<IOscillatorSource> oscillatorSource);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);
		void setPitch(float pitch);
		void setVolume(float volume);
		void setOn(bool isOn);

		void setSampleRate(float sampleRate);

		float pitch();

	private:
		void setOscillatorPitch();

		std::unique_ptr<IOscillatorSource> _oscillatorSource;
		OscillatorId _id;
		int _octaveShift;
		int _semitoneShift;
		int _centShift;
		float _volume;
		bool _isOn;
		float _originalPitch;
	};
}
