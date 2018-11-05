#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "22.10.2018"

from WaveformGenerator.Generator import Generator
import numpy as np


class SquareGenerator(Generator):
    def generate(self, length):
        t = np.arange(0, 1, 1 / length)
        output = np.zeros(length)

        output[np.where(t < 0.5)] = 1
        output[np.where(t > 0.5)] = -1

        return output

    def name(self):
        return 'Square'
