/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/envelope/EnvelopeGenerator.h"
#include "eden/AudioBuffer.h"
#include "synth/Voice.h"

namespace eden::synth::envelope
{
	EnvelopeGenerator::EnvelopeGenerator(Voice& voice)
		: _voice(&voice)
	{
	}

	void EnvelopeGenerator::applyEnvelope(eden::AudioBuffer& audioBuffer, int startSample, int samplesToProcess)
	{
		
	}
	 void EnvelopeGenerator::attack()
	 {
		 
	 }

	void EnvelopeGenerator::release()
	{
		_voice->finalizeVoice();
	}
}