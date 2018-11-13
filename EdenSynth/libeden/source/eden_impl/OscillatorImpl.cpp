/// 
/// \author Jan Wilczek
/// \date 11.11.2018
/// 
#include "eden_impl/OscillatorImpl.h"
#include "settings/Settings.h"
#include "eden/OscillatorSource.h"

namespace eden
{
	OscillatorImpl::OscillatorImpl(settings::Settings& settings, std::unique_ptr<OscillatorSource> source)
		: _ext_settings(settings)
		, _id(_ext_settings.addOscillator(source->getId()))
		, _oscillatorSource(std::move(source))
	{
	}

	OscillatorImpl::~OscillatorImpl()
	{
		_ext_settings.removeOscillator(getId());
	}

	OscillatorId OscillatorImpl::getId() const noexcept
	{
		return _id;
	}

	void OscillatorImpl::setSource(std::unique_ptr<OscillatorSource> source)
	{
		_ext_settings.setOscillatorSource(getId(), source->getId());
	}

	void OscillatorImpl::setOctaveTransposition(int octaveShift)
	{
		_ext_settings.setOctaveTransposition(getId(), octaveShift);
	}

	void OscillatorImpl::setSemitoneTransposition(int semitoneShift)
	{
		_ext_settings.setSemitoneTransposition(getId(), semitoneShift);
	}

	void OscillatorImpl::setCentTransposition(int centShift)
	{
		_ext_settings.setCentTransposition(getId(), centShift);
	}
}
