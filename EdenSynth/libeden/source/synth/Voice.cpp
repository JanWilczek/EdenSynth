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
		, _signalGenerator(std::make_shared<wavetable::SignalGenerator>(_sampleRate))
		, _subtractiveModule(std::make_shared<subtractive::SubtractiveModule>())
		, _waveshapingModule(std::make_shared<waveshaping::WaveshapingModule>())
		, _envelopeGenerator(std::make_shared<envelope::ADBDR>(_sampleRate, ADBDRParameters{}))
	{
		setBlockLength(480u);
		_envelopeGenerator->setOnEnvelopeEndedCallback([this](unsigned) { finalizeVoice(); });

		registerModules(settings);
	}

	void Voice::startNote(int midiNoteNumber, float velocity)
	{
		_currentNote = midiNoteNumber;
		_velocity = static_cast<SampleType>(velocity);

		const auto pitch = calculatePitch(_currentNote, 0);	// TODO: Handle pitch wheel.
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
			_signalGenerator->generateSignal(_innerBlock, startSample, samplesToRender);

			_subtractiveModule->process(_innerBlock, startSample, samplesToRender);

			_waveshapingModule->process(_innerBlock, startSample, samplesToRender);

			_envelopeGenerator->apply(_innerBlock, startSample, samplesToRender);

			applyVelocity(_innerBlock, startSample, samplesToRender);

			mixTo(outputBuffer, startSample, samplesToRender);

			clearInnerBlock(startSample, samplesToRender);
		}
	}

	void Voice::pitchWheelMoved(int newPitchWheelValue)
	{
		const auto newPitch = calculatePitch(_currentNote, newPitchWheelValue);
		setPitch(newPitch);
	}

	//void controllerMoved(int controllerNumber, int newControllerValue) override
	//{

	//}*/

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
		if (samplesPerBlock > _blockLength)
		{
			if (_innerBlock)
			{
				delete[] _innerBlock;
			}
			_innerBlock = new SampleType[samplesPerBlock];
		}
	}

	void Voice::finalizeVoice()
	{
		_signalGenerator->stop();
		_currentNote = -1;
	}

	double Voice::calculatePitch(int midiNoteNumber, int pitchWheelPosition)
	{
		return MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	}

	SampleType Voice::gainValue() const noexcept
	{
		return SampleType(0.1);
	}


	void Voice::setPitch(double newPitch)
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
		outputBuffer.forEachChannel([this,&startSample,&samplesToMix](SampleType* channel)
		{
			for (auto sample = startSample; sample < startSample + samplesToMix; ++sample)
			{
				channel[sample] += _innerBlock[sample];
				
				// check for clipping
				EDEN_ASSERT(channel[sample] >= -1.0 && channel[sample] <= 1.0);
			}
		});
	}

	void Voice::clearInnerBlock(int startSample, int samplesToClear)
	{
		for (auto sample = startSample; sample < startSample + samplesToClear; ++sample)
		{
			_innerBlock[sample] = SampleType(0);
		}
	}

	void Voice::registerModules(settings::Settings& settings)
	{
		settings.registerSignalGenerator(_signalGenerator);
		settings.registerSubtractiveModule(_subtractiveModule);
		settings.registerWaveshapingModule(_waveshapingModule);
		settings.registerEnvelope(_envelopeGenerator);
	}
}
