#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <vector>
#include <functional>
#include "synth/envelope/EnvelopeSegment.h"

namespace eden::synth::envelope
{
	class Envelope
	{
	public:
		typedef std::function<void()> OnEnvelopeEnded;

		virtual ~Envelope() = 0;
		virtual void apply(SampleType* channel, int startSample, int samplesToApply);
		virtual void keyOn();
		virtual void keyOff();
		virtual void setSampleRate(double sampleRate);

		void setOnEnvelopeEndedCallback(OnEnvelopeEnded callback);

	private:
		void switchToNextSegment();
		void updateGain();

		SampleType _currentGain;
		std::vector<EnvelopeSegment> _segments;
		size_t _currentSegment = 0;
		OnEnvelopeEnded _onEnvelopeEndedCallback;
	};
}
