#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>

namespace eden::synth::envelope
{
	class Envelope;
}

namespace eden::settings
{
	class EnvelopeSettings
	{
	public:
		void registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope);

	private:
		std::vector<std::shared_ptr<synth::envelope::Envelope>> _envelopes;
	};
}
