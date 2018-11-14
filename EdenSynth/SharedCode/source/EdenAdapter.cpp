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
		, _pathProvider(assetsPath)
		, _oscillator1(_synthesiser.createAndAddOscillator(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath("Sine"))))
		//, _oscillator2(_synthesiser.createAndAddOscillator(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath("Sine"))))
		//, _oscillator3(_synthesiser.createAndAddOscillator(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath("Sine"))))
		, _waveTableIndex(_pathProvider.nameToIndex("Sine"))
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
		}

		return edenMidiBuffer;
	}

	void EdenAdapter::addEdenParameters(AudioProcessorValueTreeState& pluginParameters)
	{
		// oscillator parameters
		pluginParameters.createAndAddParameter("generator.oscillator1.waveTable", "Wave table", String(), NormalisableRange<float>(0.f, static_cast<float>(_pathProvider.size() - 1u), 1.0f), static_cast<float>(_waveTableIndex),
			[this](float index) { return String(_pathProvider.indexToName(static_cast<size_t>(index))); }, 
			[this](String name) { return static_cast<float>(_pathProvider.nameToIndex(name.toStdString())); });
		pluginParameters.createAndAddParameter("generator.oscillator1.octaveTransposition", "Octave transposition", "oct", NormalisableRange<float>(-3.0f, 3.0f, 1.0f), 0.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("generator.oscillator1.semitoneTransposition", "Semitone transposition", "semit.", NormalisableRange<float>(-6.0f, 6.0f, 1.0f), 0.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("generator.oscillator1.centTransposition", "Cent transposition", "ct.", NormalisableRange<float>(-50.0f, 50.0f, 1.0f), 0.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("generator.oscillator1.volume", "Volume", String(), NormalisableRange<float>(0.f, 1.0f, 0.0001f), 0.75f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("generator.oscillator1.on", "On/Off", String(), NormalisableRange<float>(0.f, 1.f, 1.f), 1.f, nullptr, nullptr);

		// filter parameters
		pluginParameters.createAndAddParameter("filter.cutoff", "Cutoff", String(), NormalisableRange<float>(0.f, 0.5f, 0.01f), 0.49f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("filter.resonance", "Resonance", String(), NormalisableRange<float>(0.5f, 5.0f, 0.01f), 1.f, nullptr, nullptr);

		// ADBDR envelope parameters
		pluginParameters.createAndAddParameter("envelope.adbdr.attack.time", "Attack time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 30.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.attack.curve", "Attack curve", String(), NormalisableRange<float>(1.f, 2.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.decay1.time", "Decay1 time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 20.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.decay1.curve", "Decay1 curve", String(), NormalisableRange<float>(1.f, 2.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.decay2.time", "Decay2 time", "ms", NormalisableRange<float>(1.f, 50000.f, 1.f, 0.3f), 20000.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.decay2.curve", "Decay2 curve", String(), NormalisableRange<float>(1.f, 2.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.release.time", "Release time", "ms", NormalisableRange<float>(1.f, 10000.f, 1.f, 0.3f), 300.f, nullptr, nullptr);
		pluginParameters.createAndAddParameter("envelope.adbdr.release.curve", "Release curve", String(), NormalisableRange<float>(1.f, 2.f, 1.f), 1.f, nullptr, nullptr);

		pluginParameters.createAndAddParameter("envelope.adbdr.breakLevel", "Break level", String(), NormalisableRange<float>(0.f, 1.f, 0.001f, 0.7f), 0.6f, nullptr, nullptr);
	}

	void EdenAdapter::updateEdenParameters(const AudioProcessorValueTreeState& pluginParameters)
	{
		// oscillator parameters
		const auto waveTableIndex = static_cast<size_t>(float(pluginParameters.getParameterAsValue("generator.oscillator1.waveTable").getValue()));
		if (waveTableIndex != _waveTableIndex)
		{
			_waveTableIndex = waveTableIndex;
			_oscillator1->setSource(_synthesiser.createWaveTableOscillatorSource(_pathProvider.getPath(_pathProvider.indexToName(_waveTableIndex))));
		}
		_oscillator1->setOctaveTransposition(pluginParameters.getParameterAsValue("generator.oscillator1.octaveTransposition").getValue());
		_oscillator1->setSemitoneTransposition(pluginParameters.getParameterAsValue("generator.oscillator1.semitoneTransposition").getValue());
		_oscillator1->setCentTransposition(pluginParameters.getParameterAsValue("generator.oscillator1.centTransposition").getValue());

		// filter parameters
		_synthesiser.setCutoff(pluginParameters.getParameterAsValue("filter.cutoff").getValue());
		_synthesiser.setResonance(pluginParameters.getParameterAsValue("filter.resonance").getValue());

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
	}

	WaveTablePathProvider& EdenAdapter::getPathProvider()
	{
		return _pathProvider;
	}
}
