///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include "settings/EnvelopeSettings.h"
#include "synth/envelope/ADBDR.h"
#include "synth/envelope/ADSR.h"
#include "synth/envelope/Envelope.h"
#include "synth/envelope/EnvelopeFactory.h"
#include "synth/envelope/IEnvelopeHolder.h"
#include "utility/EdenAssert.h"

namespace eden::settings {
EnvelopeSettings::EnvelopeSettings(float sampleRate)
    : _currentParameters(std::make_shared<ADBDRParameters>()),
      _sampleRate(sampleRate) {}

void EnvelopeSettings::registerEnvelope(
    std::shared_ptr<synth::envelope::IEnvelopeHolder> envelope) {
  _envelopeGenerators.push_back(envelope);
}

void EnvelopeSettings::setSampleRate(float sampleRate) {
  if (sampleRate != _sampleRate) {
    _sampleRate = sampleRate;
    for (const auto envelopeGenerator : _envelopeGenerators) {
      envelopeGenerator->getEnvelope()->setSampleRate(sampleRate);
    }
  }
}

void EnvelopeSettings::setEnvelopeParameters(
    std::shared_ptr<EnvelopeParameters> envelopeParameters) {
  if (envelopeParameters->getType() == _currentParameters->getType()) {
    switch (_currentParameters->getType()) {
      case EnvelopeType::ADBDR:
        setADBDRParameters(
            std::dynamic_pointer_cast<ADBDRParameters>(envelopeParameters));
        break;
      case EnvelopeType::ADSR:
        setADSRParameters(
            std::dynamic_pointer_cast<ADSRParameters>(envelopeParameters));
        break;
      default:
        EDEN_ASSERT(false);
    }
  } else {
    // switch to new envelope
    _currentParameters = envelopeParameters;
    for (auto envelopeGenerator : _envelopeGenerators) {
      envelopeGenerator->setEnvelope(
          synth::envelope::EnvelopeFactory::createEnvelope(_sampleRate,
                                                           envelopeParameters));
    }
  }
}

void EnvelopeSettings::setADBDRParameters(
    std::shared_ptr<ADBDRParameters> adbdrParameters) {
  auto currentParameters =
      std::dynamic_pointer_cast<ADBDRParameters>(_currentParameters);

  // set envelope parameters
  if (adbdrParameters->attackTime != currentParameters->attackTime) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Attack,
                            adbdrParameters->attackTime);
    }
  }
  if (adbdrParameters->attackCurve != currentParameters->attackCurve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Attack,
                             adbdrParameters->attackCurve);
    }
  }

  if (adbdrParameters->decay1Time != currentParameters->decay1Time) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Decay1,
                            adbdrParameters->decay1Time);
    }
  }
  if (adbdrParameters->decay1Curve != currentParameters->decay1Curve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Decay1,
                             adbdrParameters->decay1Curve);
    }
  }

  if (adbdrParameters->decay2Time != currentParameters->decay2Time) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Decay2,
                            adbdrParameters->decay2Time);
    }
  }
  if (adbdrParameters->decay2Curve != currentParameters->decay2Curve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Decay2,
                             adbdrParameters->decay2Curve);
    }
  }

  if (adbdrParameters->releaseTime != currentParameters->releaseTime) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentTime(synth::envelope::ADBDR::ADBDRSegments::Release,
                            adbdrParameters->releaseTime);
    }
  }
  if (adbdrParameters->releaseCurve != currentParameters->releaseCurve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setSegmentCurve(synth::envelope::ADBDR::ADBDRSegments::Release,
                             adbdrParameters->releaseCurve);
    }
  }

  if (adbdrParameters->breakLevel != currentParameters->breakLevel) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adbdr = std::dynamic_pointer_cast<synth::envelope::ADBDR>(
          envelopeGenerator->getEnvelope());
      adbdr->setBreakLevel(adbdrParameters->breakLevel);
    }
  }

  _currentParameters = adbdrParameters;
}

void EnvelopeSettings::setADSRParameters(
    std::shared_ptr<ADSRParameters> adsrParameters) {
  auto currentParameters =
      std::dynamic_pointer_cast<ADSRParameters>(_currentParameters);

  if (adsrParameters->attackTime != currentParameters->attackTime) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSegmentTime(synth::envelope::ADSR::ADSRSegments::Attack,
                           adsrParameters->attackTime);
    }
  }

  if (adsrParameters->attackCurve != currentParameters->attackCurve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSegmentCurve(synth::envelope::ADSR::ADSRSegments::Attack,
                            adsrParameters->attackCurve);
    }
  }

  if (adsrParameters->decayTime != currentParameters->decayTime) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSegmentTime(synth::envelope::ADSR::ADSRSegments::Decay,
                           adsrParameters->decayTime);
    }
  }

  if (adsrParameters->decayCurve != currentParameters->decayCurve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSegmentCurve(synth::envelope::ADSR::ADSRSegments::Decay,
                            adsrParameters->decayCurve);
    }
  }

  if (adsrParameters->sustainLevel != currentParameters->sustainLevel) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSustainLevel(adsrParameters->sustainLevel);
    }
  }

  if (adsrParameters->releaseTime != currentParameters->releaseTime) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSegmentTime(synth::envelope::ADSR::ADSRSegments::Release,
                           adsrParameters->releaseTime);
    }
  }

  if (adsrParameters->releaseCurve != currentParameters->releaseCurve) {
    for (const auto envelopeGenerator : _envelopeGenerators) {
      auto adsr = std::dynamic_pointer_cast<synth::envelope::ADSR>(
          envelopeGenerator->getEnvelope());
      adsr->setSegmentCurve(synth::envelope::ADSR::ADSRSegments::Release,
                            adsrParameters->releaseCurve);
    }
  }

  _currentParameters = adsrParameters;
}
}  // namespace eden::settings
