#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/EnvelopeParameters.h"

namespace eden::synth::envelope
{
	class Envelope;
}

namespace eden::settings
{
	class EnvelopeSettings
	{
	public:
		EnvelopeSettings();

		void registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope);
		void setSampleRate(float sampleRate);
		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

	private:
		void setADBDRParameters(std::shared_ptr<ADBDRParameters> adbdrParameters);

		std::vector<std::shared_ptr<synth::envelope::Envelope>> _envelopes;
		std::shared_ptr<EnvelopeParameters> _currentParameters;
	};
}
