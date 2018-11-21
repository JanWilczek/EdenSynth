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

		/// <summary>
		/// Sets the wave table to be used by this wave table source.
		/// </summary>
		/// <param name="waveTable">wave table of samples to create source signal from - it should be a single cycle
		/// of a waveform of any length</param>
		void setWaveTable(WaveTable waveTable);

		std::unique_ptr<IOscillatorSource> clone() override;
		void reset() override;
		float getSample() override;
		void setPitch(float pitch) override;
		void setSampleRate(float sampleRate) override;

	private:
		float _sampleRate;

		/// <summary>
		/// Waveform to base the generated signal on.
		/// </summary>
		Waveform _waveform;
		float _omega;

		/// <summary>
		/// The increase in phase with every generated sample.
		/// </summary>
		float _phaseDeltaPerSample;

		/// <summary>
		/// Current phase in the waveform.
		/// </summary>
		float _currentPhase;
	};
}
