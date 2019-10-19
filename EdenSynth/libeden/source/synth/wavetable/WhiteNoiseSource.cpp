/// 
/// \author Jan Wilczek
/// \date 12.10.2019
/// 
#include <synth/wavetable/WhiteNoiseSource.h>
#include <random>

namespace eden::synth::wavetable
{
	WhiteNoiseSource::WhiteNoiseSource(int seed)
		: _isPlaying(false)
		, _randomEngine(seed)
		, _uniformDistribution(-1.f, 1.f)
	{
	}

	WhiteNoiseSource::WhiteNoiseSource(const WhiteNoiseSource& other)
		: _isPlaying(other._isPlaying)
		, _randomEngine(other._randomEngine)
		, _uniformDistribution(other._uniformDistribution)
	{
	}

	std::unique_ptr<IOscillatorSource> WhiteNoiseSource::clone()
	{
		return std::make_unique<WhiteNoiseSource>(*this);
	}

	void WhiteNoiseSource::reset()
	{
		_isPlaying = false;
	}

	void WhiteNoiseSource::setPitch(float pitch)
	{
		_isPlaying = true;
	}


	float WhiteNoiseSource::getSample()
	{
		return _uniformDistribution(_randomEngine);
	}

	void WhiteNoiseSource::setSampleRate(float)
	{
	}
}
