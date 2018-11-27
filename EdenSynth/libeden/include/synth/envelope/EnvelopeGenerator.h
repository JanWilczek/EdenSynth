#pragma once
/// 
/// \author Jan Wilczek
/// \date 27.11.2018
/// 
#include "synth/envelope/Envelope.h"

namespace eden::synth::envelope
{
	class EnvelopeGenerator
	{
	public:
		EnvelopeGenerator(std::shared_ptr<Envelope> envelope);

		/// <summary>
		/// Applies envelope on samples in channel in range [<paramref name="startSample"/>, <paramref name="startSample"/> + <paramref name="samplesToApply"/>).
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToApply"></param>
		void apply(float* channel, int startSample, int samplesToApply);

		void keyOn();
		void keyOff();

		std::shared_ptr<Envelope> getEnvelope() const noexcept;

		void setEnvelope(std::shared_ptr<Envelope> envelope);
		/// <summary>
		/// Sets the function to be called when the envelope has ended.
		/// Sample index at which the envelope has ended is passed to it.
		/// </summary>
		/// <param name="callback"></param>
		void setOnEnvelopeEndedCallback(Envelope::OnEnvelopeEnded callback);

	private:
		std::shared_ptr<Envelope> _envelope;
		Envelope::OnEnvelopeEnded _callback;
	};
}
