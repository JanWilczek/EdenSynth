/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "EdenAdapter.h"
#include "eden/MidiBuffer.h"
#include "eden/EdenSynthesiser.h"
#include "eden/EnvelopeParameters.h"

namespace eden_vst
{
	EdenAdapter::EdenAdapter(eden::EdenSynthesiser& synthesiser, std::experimental::filesystem::path assetsPath)
 		: _synthesiser(synthesiser)
		, _oscillators(_synthesiser, WaveTablePathProvider(assetsPath), 3u)
		, _filterParameters(_synthesiser)
		, _waveshapingParameters(_synthesiser)
	{
	}

	eden::MidiBuffer EdenAdapter::convertToEdenMidi(const juce::MidiBuffer& juceMidiBuffer)
	{
		eden::MidiBuffer edenMidiBuffer;

		int midiEventPos;
		MidiMessage m;

		juce::MidiBuffer::Iterator midiIterator(juceMidiBuffer);

		while (midiIterator.getNextEvent(m, midiEventPos))
		{
			if (m.isNoteOn())
			{
				eden::MidiMessage edenMessage(m.getChannel(), eden::MidiMessage::MidiMessageType::NoteOn);
				edenMessage.setVelocity(m.getFloatVelocity());
				edenMessage.setNoteNumber(m.getNoteNumber());
				edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
			}
			else if (m.isNoteOff())
			{
				eden::MidiMessage edenMessage(m.getChannel(), eden::MidiMessage::MidiMessageType::NoteOff);
				edenMessage.setVelocity(m.getFloatVelocity());
				edenMessage.setNoteNumber(m.getNoteNumber());
				edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
			}
			else if (m.isPitchWheel())
			{
				eden::MidiMessage edenMessage(m.getChannel(), eden::MidiMessage::MidiMessageType::PitchBendChange);
				edenMessage.setPitchWheelPosition(m.getPitchWheelValue());
				edenMidiBuffer.addEvent(std::move(edenMessage), midiEventPos);
			}
		}

		return edenMidiBuffer;
	}

	void EdenAdapter::addEdenParameters(AudioProcessorValueTreeState& pluginParameters)
	{
		// general parameters
		pluginParameters.createAndAddParameter("pitchBend.semitonesDown", "Pitch bend semitones down", String(), NormalisableRange<float>(-24.f, 0.f, 1.f), -12.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("pitchBend.semitonesUp", "Pitch bend semitones up", String(), NormalisableRange<float>(0.f, 24.f, 1.f), 2.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("frequencyOfA4", "Frequency of A4", "Hz", NormalisableRange<float>(400.f, 500.f, 0.1f), 440.f, nullptr, nullptr);

		// oscillator parameters
		_oscillators.addOscillatorParameters(pluginParameters);

		// filter parameters
		_filterParameters.addFilterParameters(pluginParameters);

		// waveshaping parameters
		_waveshapingParameters.addWaveshapingParameters(pluginParameters);

		// ADBDR envelope parameters
		pluginParameters.createAndAddParameter("envelope.adbdr.attack.time", "Attack time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 30.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.attack.curve", "Attack curve", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.decay1.time", "Decay1 time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 20.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.decay1.curve", "Decay1 curve", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.decay2.time", "Decay2 time", "ms", NormalisableRange<float>(1.f, 100000.f, 1.f, 0.3f), 20000.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.decay2.curve", "Decay2 curve", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.release.time", "Release time", "ms", NormalisableRange<float>(1.f, 40000.f, 1.f, 0.3f), 300.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.release.curve", "Release curve", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.breakLevel", "Break level", String(), NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f), 0.6f, nullptr, nullptr);

		// output parameters
		pluginParameters.createAndAddParameter("output.volume", "Global volume", String(), NormalisableRange<float>(0.f, 1.f, 0.001f, 0.4f), 1.0f, nullptr, nullptr);
	}

	void EdenAdapter::updateEdenParameters(const AudioProcessorValueTreeState& pluginParameters)
	{
		// general parameters
		_synthesiser.setPitchBendRange({ static_cast<int>(*pluginParameters.getRawParameterValue("pitchBend.semitonesDown")), static_cast<int>(*pluginParameters.getRawParameterValue("pitchBend.semitonesUp")) });
		_synthesiser.setFrequencyOfA4(*pluginParameters.getRawParameterValue("frequencyOfA4"));

		// oscillator parameters
		_oscillators.updateOscillatorParameters(pluginParameters);

		// filter parameters
		_filterParameters.updateFilterParameters();

		// waveshaping parameters
		_waveshapingParameters.updateWaveshapingParameters();

		// ADBDR envelope parameters
		{
			eden::ADBDRParameters adbdrParameters;

			adbdrParameters.attackTime = std::chrono::milliseconds(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.attack.time").getValue()));
			adbdrParameters.attackCurve = static_cast<eden::EnvelopeSegmentCurve>(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.attack.curve").getValue()));

			adbdrParameters.decay1Time = std::chrono::milliseconds(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.decay1.time").getValue()));
			adbdrParameters.decay1Curve = static_cast<eden::EnvelopeSegmentCurve>(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.decay1.curve").getValue()));

			adbdrParameters.decay2Time = std::chrono::milliseconds(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.decay2.time").getValue()));
			adbdrParameters.decay2Curve = static_cast<eden::EnvelopeSegmentCurve>(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.decay2.curve").getValue()));

			adbdrParameters.releaseTime = std::chrono::milliseconds(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.release.time").getValue()));
			adbdrParameters.releaseCurve = static_cast<eden::EnvelopeSegmentCurve>(static_cast<int>(pluginParameters.getParameterAsValue("envelope.adbdr.release.curve").getValue()));

			adbdrParameters.breakLevel = pluginParameters.getParameterAsValue("envelope.adbdr.breakLevel").getValue();

			_synthesiser.setEnvelopeParameters(std::make_shared<eden::ADBDRParameters>(adbdrParameters));
		}

		// output parameters
		_synthesiser.setVolume(*pluginParameters.getRawParameterValue("output.volume"));
	}

	const WaveTablePathProvider& EdenAdapter::getPathProvider() const
	{
		return _oscillators.getPathProvider();
	}

	std::shared_ptr<WaveshapingTransferFunctionContainer> EdenAdapter::getWaveshapingTransferFunction() const noexcept
	{
		return _waveshapingParameters.getTransferFunctionContainer();
	}
}
