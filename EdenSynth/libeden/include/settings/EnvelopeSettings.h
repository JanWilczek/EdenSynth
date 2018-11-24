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
	/// <summary>
	/// Class responsible for changing settings of note envelopes belonging to all voices.
	/// </summary>
	class EnvelopeSettings
	{
	public:
		EnvelopeSettings();

		/// <summary>
		/// Registers an envelope for settings' control.
		/// </summary>
		/// <param name="envelope"></param>
		void registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope);

		/// <summary>
		/// Sets sample rate of all registered envelopes.
		/// </summary>
		/// <param name="sampleRate"></param>
		void setSampleRate(float sampleRate);

		/// <summary>
		/// Sets envelope parameters of all registered envelopes.
		/// </summary>
		/// <param name="envelopeParameters"></param>
		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

	private:
		/// <summary>
		/// Sets envelope settings with knowledge that they are parameters of an ADBDR envelope.
		/// </summary>
		/// <param name="adbdrParameters"></param>
		void setADBDRParameters(std::shared_ptr<ADBDRParameters> adbdrParameters);

		/// <summary>
		/// All envelopes registered.
		/// </summary>
		std::vector<std::shared_ptr<synth::envelope::Envelope>> _envelopes;

		/// <summary>
		/// Currently set envelope parameters.
		/// </summary>
		std::shared_ptr<EnvelopeParameters> _currentParameters;
	};
}
