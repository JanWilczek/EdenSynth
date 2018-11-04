#pragma once
/// 
/// \author Jan Wilczek
/// \date 04.11.2018
/// 
#include <chrono>
#include "SampleType.h"

namespace eden
{
	using namespace std::chrono_literals;

	enum class EnvelopeSegmentCurve
	{
		Linear,
		Exponential
	};
	
	struct EnvelopeParameters
	{
		virtual ~EnvelopeParameters(){}
	};

	struct ADBDRParameters : public EnvelopeParameters
	{
		
		~ADBDRParameters(){}

		std::chrono::milliseconds attackTime = 50ms;
		EnvelopeSegmentCurve attackCurve = EnvelopeSegmentCurve::Exponential;

		std::chrono::milliseconds decay1Time = 100ms;
		EnvelopeSegmentCurve decay1Curve = EnvelopeSegmentCurve::Exponential;
		
		std::chrono::milliseconds decay2Time = 20000ms;
		EnvelopeSegmentCurve decay2Curve = EnvelopeSegmentCurve::Exponential;

		std::chrono::milliseconds releaseTime = 1000ms;
		EnvelopeSegmentCurve releaseCurve = EnvelopeSegmentCurve::Exponential;

		SampleType breakLevel = SampleType(0.8f);
	};
}
