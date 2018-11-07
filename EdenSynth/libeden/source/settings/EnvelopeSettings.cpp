/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/EnvelopeSettings.h"
#include "synth/envelope/Envelope.h"
#include "utility/EdenAssert.h"

namespace eden::settings
{
	EnvelopeSettings::EnvelopeSettings()
		: _currentParameters(std::make_shared<ADBDRParameters>())
	{
	}

	void EnvelopeSettings::registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope)
	{
		_envelopes.push_back(envelope);
	}

	void EnvelopeSettings::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters)
	{
		if (envelopeParameters->type == _currentParameters->type)
		{
			switch (_currentParameters->type)
			{
			case EnvelopeType::ADBDR:
				setADBDRParameters(std::dynamic_pointer_cast<ADBDRParameters>(envelopeParameters));
				break;
			default:
				EDEN_ASSERT(false);
			}
		}
		else
		{
			// switch to new envelope
		}
	}

	void EnvelopeSettings::setADBDRParameters(std::shared_ptr<ADBDRParameters> adbdrParameters)
	{
		auto currentParameters = std::dynamic_pointer_cast<ADBDRParameters>(_currentParameters);

		// set envelope parameters
		for (auto envelope : _envelopes)
		{
		}
	}
}
