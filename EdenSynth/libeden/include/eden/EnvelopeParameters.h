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

	struct ADBDRParameters : EnvelopeParameters
	{
		ADBDRParameters() {};
		
		ADBDRParameters(std::chrono::milliseconds _attackTime, EnvelopeSegmentCurve _attackCurve,
			std::chrono::milliseconds _decay1Time, EnvelopeSegmentCurve _decay1Curve,
			std::chrono::milliseconds _decay2Time, EnvelopeSegmentCurve _decay2Curve,
			std::chrono::milliseconds _releaseTime, EnvelopeSegmentCurve _releaseCurve,
			SampleType _breakLevel) : attackTime(_attackTime), attackCurve(_attackCurve),
		decay1Time(_decay1Time), decay1Curve(_decay1Curve), decay2Time(_decay2Time), decay2Curve(_decay2Curve),
		releaseTime(_releaseTime), releaseCurve(_releaseCurve), breakLevel(_breakLevel)
		{}

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
