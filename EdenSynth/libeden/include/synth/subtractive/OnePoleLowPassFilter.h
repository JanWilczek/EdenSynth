#pragma once
/// 
/// \author Jan Wilczek
/// \date 25.11.2018
/// 

namespace eden::synth::subtractive
{
	class OnePoleLowPassFilter
	{
	public:
		OnePoleLowPassFilter(float sampleRate);
		float processSample(float sample);
		void setCutoffFrequency(float cutoffFrequency);
		void setSampleRate(float sampleRate);

	private:
		void calculateG();

		float _sampleRate;

		float _g;
		float _cutoffFrequency;

		float _feedforwardSample;
		float _feedbackSample;
	};
}
