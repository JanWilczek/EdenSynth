#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "synth/wavetable/IOscillatorSource.h"
#include "synth/wavetable/Waveform.h"

namespace eden::synth::wavetable
{
	class WaveTableSource : public IOscillatorSource
	{
	public:
		WaveTableSource(float sampleRate);
		WaveTableSource(const WaveTableSource&);
		~WaveTableSource() override = default;


		std::unique_ptr<IOscillatorSource> clone() override;

		void setWaveTable(WaveTable waveTable);

		SampleType getSample() override;
		void setPitch(float pitch) override;
		void setSampleRate(float sampleRate) override;

	private:
		float _sampleRate;
		Waveform _waveform;
		float _phaseDeltaPerSample = 0.f;
		float _currentPhase = 0.f;
	};
}
