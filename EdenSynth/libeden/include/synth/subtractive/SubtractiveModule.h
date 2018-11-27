#pragma once
///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///
#include "synth/IMonoModule.h"
#include "synth/subtractive/MoogFilter.h"
#include "synth/envelope/Envelope.h"

namespace eden 
{
	enum class PassbandAttenuation;
}

namespace eden::synth::subtractive
{
	class SubtractiveModule : public IMonoModule
	{
	public:
		explicit SubtractiveModule(float sampleRate);
		~SubtractiveModule() override = default;

		void process(float* audioChannel, int startSample, int samplesToProcess) override;

		void keyOn();
		void keyOff();

		void setCutoff(float cutoff);
		void setResonance(float resonance);
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);
		void setSampleRate(float sampleRate);
		void setPitch(float pitch);
		
	private:
		void setCutoffFrequency();

		MoogFilter _filter;
		std::shared_ptr<envelope::Envelope> _filterEnvelope;
		float _cutoff;
		float _pitch;
	};
}
