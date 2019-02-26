#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.02.2019
/// 
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable
{
	/// <summary>
	/// Oscillator generating a Virtual Analog sawtooth waveform based on the 
	/// Differential Parabolic Waveform (DPW) algorithm proposed by Valimaki and Huovilainen.
	/// </summary>
	class SawtoothVASource : public IOscillatorSource
	{
	public:
		SawtoothVASource(float sampleRate);
		~SawtoothVASource() override = default;
		SawtoothVASource(const SawtoothVASource& other);

		/// <summary>
		/// Sets phase shift in relation to the whole cycle. 
		/// For example, 0.5 moves the waveform in phase by pi.
		/// </summary>
		/// <param name="phaseShift"></param>
		void setPhaseShift(float phaseShift);

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
		float _z2;
		float _c;
		float _pitch;
		float _phaseShift;
	};
}
