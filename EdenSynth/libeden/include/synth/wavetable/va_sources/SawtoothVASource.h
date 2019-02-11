#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.02.2019
/// 
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable
{
	class SawtoothVASource : public IOscillatorSource
	{
	public:
		SawtoothVASource();
		~SawtoothVASource() override = default;
		SawtoothVASource(const SawtoothVASource& other);

		std::unique_ptr<IOscillatorSource> clone() override;

		void reset() override;

		void setPitch(float pitch) override;

		float getSample() override;

		void setSampleRate(float sampleRate) override;

	private:
		float _sampleRate;
		float _phase;
		float _delta;
		float _z1;
		float _c;
		float _pitch;
	};
}
