#pragma once
/// 
/// \author Jan Wilczek
/// \date 04.11.2018
/// 
#include <memory>

namespace eden
{
	struct EnvelopeParameters;
	struct ADBDRParameters;
}

namespace eden::synth::envelope
{
	class Envelope;
	class ADBDR;

	class EnvelopeFactory
	{
	public:
		/// <summary>
		/// Creates envelope in a polymorphic way depending on the passed <paramref name="parameters"/>.
		/// </summary>
		/// <param name="sampleRate"></param>
		/// <param name="parameters">polymorphic parameters on which the created envelope is 
		/// based - e.g. pointer to ADBDRParameters class to create an ADBDR envelope</param>
		/// <returns>the created concrete envelope</returns>
		static std::unique_ptr<Envelope> createEnvelope(double sampleRate, std::shared_ptr<EnvelopeParameters> parameters);

	private:
		/// <param name="sampleRate"></param>
		/// <param name="parameters"></param>
		/// <returns>the ADBDR envelope created with the supplied parameters</returns>
		static std::unique_ptr<Envelope> createADBDREnvelope(double sampleRate, std::shared_ptr<ADBDRParameters> parameters);
	};
}
