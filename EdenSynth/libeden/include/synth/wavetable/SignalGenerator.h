#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include "Waveform.h"

namespace eden::synth::wavetable
{
	class SignalGenerator
	{
	public:
		explicit SignalGenerator(double sampleRate);

		void generateSignal(SampleType* audioChannel, int startSample, int samplesToGenerate);
		void stop();
		void setPitch(double pitch);
		void setSampleRate(double sampleRate);

	private:
		void generateSample(SampleType* audioChannel, int sampleIndex);

		double _currentPhase = 0.0;
		double _phaseDeltaPerSample = 0.0;
		double _sampleRate;
		Waveform _signalSource;
	};
}
