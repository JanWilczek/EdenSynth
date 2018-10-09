#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include "eden/AudioBuffer.h"

namespace eden::synth::wavetable
{
	class SignalGenerator
	{
	public:
		explicit SignalGenerator(double sampleRate);

		void generateSignal(AudioBuffer::SampleType* audioChannel, int startSample, int samplesToGenerate);
		void stop();
		void setPitch(double pitch);
		void setSampleRate(double sampleRate);

	private:
		void generateSample(AudioBuffer::SampleType* audioChannel, int sampleIndex);

		double _currentPhase = 0.0;
		double _phaseDeltaPerSample = 0.0;
		double _sampleRate;
		//SignalSource _signalSource;
		//IInterpolator _interpolator;
	};
}