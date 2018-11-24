/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "eden/OscillatorSource.h"

namespace eden
{
	OscillatorSource::OscillatorSource(std::unique_ptr<OscillatorSourceImpl> impl)
		: _impl(std::move(impl))
	{
	}

	OscillatorSourceId OscillatorSource::getId()
	{
		return _impl->getId();
	}
}
