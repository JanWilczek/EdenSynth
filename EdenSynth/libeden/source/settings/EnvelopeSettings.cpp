/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/EnvelopeSettings.h"
#include "synth/envelope/Envelope.h"
#include "utility/EdenAssert.h"
#include "synth/envelope/ADBDR.h"

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

	void EnvelopeSettings::setSampleRate(float sampleRate)
	{
		for (auto envelope : _envelopes)
		{
			envelope->setSampleRate(sampleRate);
		}
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
			// switch to new envelope - TODO: unimplemented.
			EDEN_ASSERT(false);
		}
	}

	void EnvelopeSettings::setADBDRParameters(std::shared_ptr<ADBDRParameters> adbdrParameters)
	{
		auto currentParameters = std::dynamic_pointer_cast<ADBDRParameters>(_currentParameters);

		// set envelope parameters
		if (adbdrParameters->attackTime != currentParameters->attackTime)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Attack, adbdrParameters->attackTime);
			}
		}
		if (adbdrParameters->attackCurve != currentParameters->attackCurve)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Attack, adbdrParameters->attackCurve);
			}
		}

		if (adbdrParameters->decay1Time != currentParameters->decay1Time)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Decay1, adbdrParameters->decay1Time);
			}
		}
		if (adbdrParameters->decay1Curve != currentParameters->decay1Curve)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Decay1, adbdrParameters->decay1Curve);
			}
		}

		if (adbdrParameters->decay2Time != currentParameters->decay2Time)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Decay2, adbdrParameters->decay2Time);
			}
		}
		if (adbdrParameters->decay2Curve != currentParameters->decay2Curve)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Decay2, adbdrParameters->decay2Curve);
			}
		}

		if (adbdrParameters->releaseTime != currentParameters->releaseTime)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Release, adbdrParameters->releaseTime);
			}
		}
		if (adbdrParameters->releaseCurve != currentParameters->releaseCurve)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Release, adbdrParameters->releaseCurve);
			}
		}

		if (adbdrParameters->breakLevel != currentParameters->breakLevel)
		{
			for (auto envelope : _envelopes)
			{
				auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(envelope);
				adbdr->setBreakLevel(adbdrParameters->breakLevel);
			}
		}

		_currentParameters = adbdrParameters;
	}
}
