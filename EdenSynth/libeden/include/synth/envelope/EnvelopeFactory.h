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
		static std::unique_ptr<Envelope> createEnvelope(double sampleRate, std::shared_ptr<EnvelopeParameters> parameters);

	private:
		static std::unique_ptr<Envelope> createADBDREnvelope(double sampleRate, std::shared_ptr<ADBDRParameters> parameters);
	};
}
