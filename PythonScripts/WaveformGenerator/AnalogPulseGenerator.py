#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "26.10.2018"

import numpy as np
from WaveformGenerator.Generator import Generator
from WaveformGenerator.AnalogSawtoothGenerator import AnalogSawtoothGenerator
from WaveformGenerator.utils import limit, scale_to_range


class AnalogPulseGenerator(Generator):
    def __init__(self, duty_cycle=0.5):
        self.__duty_cycle = duty_cycle

    def generate(self, length):
        sawtooth_generator = AnalogSawtoothGenerator()
        saw1 = sawtooth_generator.generate(length)
        saw2 = np.roll(saw1, int(len(saw1) * self.__duty_cycle))
        square = saw1 - saw2

        scale_to_range(square)
        limit(square)

        return square

    def name(self):
        return 'AnalogPulse{:.0f}Cycle'.format(self.__duty_cycle * 100)
