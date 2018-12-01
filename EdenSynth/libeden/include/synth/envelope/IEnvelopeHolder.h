#pragma once
/// 
/// \author Jan Wilczek
/// \date 01.12.2018
/// 
#include <memory>

namespace eden::synth::envelope
{
	class Envelope;

	class IEnvelopeHolder
	{
	public:
		virtual ~IEnvelopeHolder() = 0;
		virtual void setEnvelope(std::shared_ptr<Envelope> envelope) = 0;
		virtual std::shared_ptr<Envelope> getEnvelope() const noexcept = 0;
	};
	inline IEnvelopeHolder::~IEnvelopeHolder(){}
}
