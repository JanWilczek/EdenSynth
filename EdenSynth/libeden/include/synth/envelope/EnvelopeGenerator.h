#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 

namespace eden
{
	class AudioBuffer;

	namespace synth
	{
		class Voice;
	}
}

namespace eden::synth::envelope
{
	class EnvelopeGenerator
	{
	public:
		explicit EnvelopeGenerator(Voice& voice);

		void attack();
		void release();
		void applyEnvelope(AudioBuffer& audioBuffer, int startSample, int samplesToProcess);

	private:
		Voice* _voice;
	};
}
