#pragma once
///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///
#include "synth/IMonoModule.h"
#include "synth/subtractive/MoogFilter.h"
#include "synth/envelope/Envelope.h"
#include "synth/Envelope/IEnvelopeHolder.h"

namespace eden 
{
	enum class PassbandAttenuation;
}

namespace eden::synth::subtractive
{
	class SubtractiveModule : public IMonoModule, public envelope::IEnvelopeHolder
	{
	public:
		explicit SubtractiveModule(float sampleRate);
		~SubtractiveModule() override = default;

		void process(float* audioChannel, int startSample, int samplesToProcess) override;

		void keyOn();
		void keyOff();

		void setCutoff(float cutoff);
		void setResonance(float resonance);
		void setContourAmount(float contourAmount);
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);
		void setEnvelope(std::shared_ptr<envelope::Envelope> envelope) override;
		std::shared_ptr<envelope::Envelope> getEnvelope() const noexcept override;
		void setSampleRate(float sampleRate);
		void setPitch(float pitch);
		
	private:
		void setCutoffFrequency();

		MoogFilter _filter;
		std::shared_ptr<envelope::Envelope> _filterEnvelope;
		
		/// <summary>
		/// Number of harmonic to set the cutoff frequency at.
		/// </summary>
		float _cutoff;

		/// <summary>
		/// Determines the factor by which transform the filter envelope. 
		/// If envelope's minimum is 0 and maximum is 1 then after transformation the minimum will be
		/// (1 - <c>_contourAmount</c>) and maximum 1. The factor is thus responsible for amplitude scale of the envelope.
		/// </summary>
		float _contourAmount;
		float _pitch;
	};
}
