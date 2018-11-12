/// 
/// \author Jan Wilczek
/// \date 11.11.2018
/// 
#include "eden_impl/OscillatorImpl.h"
#include "settings/Settings.h"
#include "eden/OscillatorSource.h"
#include "synth/wavetable/SynthOscillator.h"

namespace eden
{
	OscillatorImpl::OscillatorImpl(settings::Settings& settings, std::unique_ptr<OscillatorSource> source)
		: _ext_settings(settings)
		, _id(_ext_settings.getAvailableOscillatorId())
		, _oscillatorSource(std::move(source))
	{
		synth::wavetable::SynthOscillator synthOscillator{};
		_ext_settings.addOscillator(synthOscillator);
	}

	OscillatorId OscillatorImpl::getId() const noexcept
	{
		return _id;
	}

	void OscillatorImpl::setSource(std::unique_ptr<OscillatorSource> source)
	{
		
	}

	void OscillatorImpl::setOctaveTransposition(int octaveShift)
	{
		
	}

	void OscillatorImpl::setSemitoneTransposition(int semitoneShift)
	{
		
	}

	void OscillatorImpl::setCentTransposition(int centShift)
	{
		
	}
}
