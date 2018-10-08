#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 

namespace eden
{
	class AudioBuffer;
}

namespace eden::synth::envelope
{
	class EnvelopeGenerator
	{
		void applyEnvelope(eden::AudioBuffer& audioBuffer, int startSample, int samplesToProcess);
	};
}
