#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.10.2018
/// 
#include <vector>
#include <functional>

namespace eden::synth::envelope
{
	class EnvelopeSegment;

	/// <summary>
	/// Abstract class representing time envelope.
	/// Subclasses can consist of multiple envelope segments.
	/// "Level" in envelope classes refers to linear value (in range [-1, 1]).
	/// </summary>
	class Envelope
	{
	public:
		/// <summary>
		/// Type of function invoked when the envelope has ended (i.e. gain is 0 again).
		/// The sample index of envelope's end should be passed to it.
		/// </summary>
		typedef std::function<void(unsigned)> OnEnvelopeEnded;

		virtual ~Envelope() = 0;

		/// <summary>
		/// Applies envelope on samples in channel in range [startSample, samplesToApply] (closed).
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToApply"></param>
		virtual void apply(float* channel, int startSample, int samplesToApply);

		/// <summary>
		/// Handles note on event - typically starts attack segment.
		/// </summary>
		virtual void keyOn() = 0;

		/// <summary>
		/// Handles note off event - typically starts release segment.
		/// </summary>
		virtual void keyOff() = 0;

		/// <summary>
		/// Checks if the envelope has ended and applies appropriate actions if it did.
		/// </summary>
		/// <param name="currentSampleIndex"></param>
		virtual void checkForEnd(unsigned currentSampleIndex) = 0;

		/// <summary>
		/// Sets sample rate.
		/// </summary>
		/// <param name="sampleRate"></param>
		virtual void setSampleRate(double sampleRate);

		/// <summary>
		/// Sets the function to be called when the envelope has ended.
		/// Sample index at which the envelope has ended is passed to it.
		/// </summary>
		/// <param name="callback"></param>
		void setOnEnvelopeEndedCallback(OnEnvelopeEnded callback);

	protected:
		/// <summary>
		/// Switches to given envelope segment.
		/// </summary>
		/// <param name="segment"></param>
		void switchToSegment(size_t segment);

		/// <summary>
		/// Updates current gain value.
		/// </summary>
		void updateGain();

		/// <summary>
		/// Current envelope value (linear).
		/// </summary>
		float _currentLevel = 0.0;

		/// <summary>
		/// Envelope's segments.
		/// </summary>
		std::vector<EnvelopeSegment*> _segments;

		/// <summary>
		/// Current envelope segment.
		/// </summary>
		size_t _currentSegment = 0;

		/// <summary>
		/// Callback to call after envelope has ended.
		/// </summary>
		OnEnvelopeEnded _onEnvelopeEndedCallback = [](unsigned){};
	};
}
