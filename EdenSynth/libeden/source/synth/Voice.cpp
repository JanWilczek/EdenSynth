/// 
/// \author Jan Wilczek
/// \date 03.09.2018
/// 
#include "synth/Voice.h"

#include "eden/AudioBuffer.h"
#include "eden/MidiMessage.h"
#include "synth/envelope/ADBDR.h"
#include "utility/EdenAssert.h"
#include "eden/EnvelopeParameters.h"
#include "settings/Settings.h"

namespace eden::synth
{
	Voice::Voice(settings::Settings& settings, double sampleRate)
		: _sampleRate(sampleRate)
		, _innerBuffer(1, 480u)
		, _signalGenerator(std::make_shared<wavetable::SignalGenerator>())
		, _subtractiveModule(std::make_shared<subtractive::SubtractiveModule>())
		, _waveshapingModule(std::make_shared<waveshaping::WaveshapingModule>())
		, _envelopeGenerator(std::make_shared<envelope::ADBDR>(_sampleRate, ADBDRParameters{}))
	{
		_envelopeGenerator->setOnEnvelopeEndedCallback([this](unsigned) { finalizeVoice(); });

		registerModules(settings);
	}

	void Voice::startNote(int midiNoteNumber, float velocity)
	{
		_currentNote = midiNoteNumber;
		_velocity = static_cast<SampleType>(velocity);

		const auto pitch = calculatePitch(_currentNote, 0);
		setPitch(pitch);

		_envelopeGenerator->keyOn();
	}

	void Voice::stopNote(float /* velocity */)
	{
		_envelopeGenerator->keyOff();
	}

	void Voice::renderBlock(AudioBuffer& outputBuffer, int startSample, int samplesToRender)
	{
		if (isPlaying())
		{
			prepareToRender(startSample, samplesToRender);

			_signalGenerator->generateSignal(_innerBuffer.getWritePointer(0), startSample, samplesToRender);

			_subtractiveModule->process(_innerBuffer.getWritePointer(0), startSample, samplesToRender);

			_waveshapingModule->process(_innerBuffer.getWritePointer(0), startSample, samplesToRender);

			_envelopeGenerator->apply(_innerBuffer.getWritePointer(0), startSample, samplesToRender);

			applyVelocity(_innerBuffer.getWritePointer(0), startSample, samplesToRender);

			mixTo(outputBuffer, startSample, samplesToRender);
		}
	}

	void Voice::setPitchBend(int pitchBendInSemitones)
	{
		_signalGenerator->setPitchBend(pitchBendInSemitones);
		_subtractiveModule->setPitchBend(pitchBendInSemitones);
	}

	bool Voice::isPlaying() const noexcept
	{
		return _currentNote != -1;
	}

	bool Voice::isPlayingNote(const int midiNoteNumber) const noexcept
	{
		return _currentNote == midiNoteNumber;
	}

	double Voice::getSampleRate() const noexcept
	{
		return _sampleRate;
	}

	void Voice::setSampleRate(double newSampleRate)
	{
		_sampleRate = newSampleRate;
		_signalGenerator->setSampleRate(_sampleRate);
		_envelopeGenerator->setSampleRate(_sampleRate);
	}

	void Voice::setBlockLength(unsigned samplesPerBlock)
	{
		_innerBuffer.setNumSamples(samplesPerBlock);
	}

	void Voice::finalizeVoice()
	{
		_signalGenerator->stop();
		_currentNote = -1;
	}

		SampleType Voice::gainValue() const noexcept
	{
		return SampleType(0.2);
	}

	void Voice::setPitch(float newPitch)
	{
		_signalGenerator->setPitch(newPitch);

		_subtractiveModule->setPitch(newPitch);
	}

	void Voice::applyVelocity(SampleType* channel, int startSample, int samplesToRender)
	{
		for (int sample = startSample; sample < startSample + samplesToRender; ++sample)
		{
			channel[sample] *= gainValue() * _velocity;

			// check for clipping
			EDEN_ASSERT(channel[sample] >= -1.0 && channel[sample] <= 1.0);
		}
	}

	void Voice::mixTo(AudioBuffer& outputBuffer, int startSample, int samplesToMix)
	{
		outputBuffer.forEachChannel([this, &startSample, &samplesToMix](SampleType* channel)
		{
			for (auto sample = startSample; sample < startSample + samplesToMix; ++sample)
			{
				//channel[sample] += _innerBlock[sample];
				channel[sample] += _innerBuffer.getReadPointer(0)[sample];

				// check for clipping
				EDEN_ASSERT(channel[sample] >= -1.0 && channel[sample] <= 1.0);
			}
		});
	}

	void Voice::prepareToRender(int startSample, int samplesToRender)
	{
		_innerBuffer.fillFromTo(SampleType(0), startSample, startSample + samplesToRender);
		_innerBuffer.setNumSamples(samplesToRender);
	}

	void Voice::registerModules(settings::Settings& settings)
	{
		settings.registerSignalGenerator(_signalGenerator);
		settings.registerSubtractiveModule(_subtractiveModule);
		settings.registerWaveshapingModule(_waveshapingModule);
		settings.registerEnvelope(_envelopeGenerator);
	}
}
