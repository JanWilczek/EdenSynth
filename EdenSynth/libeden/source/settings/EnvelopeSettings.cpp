/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/EnvelopeSettings.h"
#include "synth/envelope/Envelope.h"

namespace eden::settings
{
	void EnvelopeSettings::registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope)
	{
		_envelopes.push_back(envelope);
	}
}
