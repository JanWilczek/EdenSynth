#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "22.10.2018"

from WaveformGenerator.Generator import Generator
import numpy as np


class SineGenerator(Generator):
    def generate(self, length):
        t = np.arange(0, 1, 1 / length)

        output = np.sin(2 * np.pi * t)

        return output

    def name(self):
        return 'Sine'
