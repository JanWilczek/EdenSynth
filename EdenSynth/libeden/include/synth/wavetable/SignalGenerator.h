#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 

namespace eden
{
	class AudioBuffer;
}

namespace eden::synth::wavetable
{
	class SignalGenerator
	{
	public:
		void generateSignal(eden::AudioBuffer& audioBuffer, int startSample, int samplesToGenerate);
		void setPitch(double pitch);
	};
}