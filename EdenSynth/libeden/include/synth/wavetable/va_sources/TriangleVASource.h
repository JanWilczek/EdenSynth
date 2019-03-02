#pragma once
/// 
/// \author Jan Wilczek
/// \date 27.02.2019
/// 
#include "synth/wavetable/IOscillatorSource.h"
#include "synth/wavetable/va_sources/SineVASource.h"

namespace eden::synth::wavetable
{
	class TriangleVASource : public IOscillatorSource
	{
	public:
		TriangleVASource(float sampleRate);
		~TriangleVASource() = default;
		TriangleVASource(const TriangleVASource& other);

		std::unique_ptr<IOscillatorSource> clone() override;
		void reset() override;
		void setPitch(float pitch) override;
		float getSample() override;
		void setSampleRate(float sampleRate) override;

	private:
		float _sampleRate;
		SineVASource _sine;
		float _z1;
		float _c;
		bool _isPlaying;
	};
}
