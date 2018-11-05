#!/usr/bin/env/python
__author__ = "Jan Wilczek"
__date__ = "28.10.2018"

import numpy as np
from WaveformGenerator.Generator import Generator
from WaveformGenerator.utils import resample, scale_to_range


class AnalogSineGenerator(Generator):
    def generate(self, length):
        freq = 2793.8
        fs = 44100
        nb_repetitions = 7

        delta = 2 * freq / fs

        phase = np.arange(0, nb_repetitions, delta)
        mod_phase = phase % 1
        bphase = 2 * mod_phase - 1
        sq = np.power(bphase, 2)

        reversed_bphase = 1 - sq

        square = np.ones(len(reversed_bphase))
        sign = -1
        for i in range(0, len(square)):
            if bphase[i] < 0 < bphase[i-1]:
                sign *= -1
            square[i] *= sign

        modulated_signal = np.multiply(reversed_bphase, square)

        resampled = resample(modulated_signal, fs_in=round(fs / freq), fs_out=length)

        zero_crossings = np.where(np.diff(np.sign(resampled)))[0]

        one_cycle = resampled[zero_crossings[4]:zero_crossings[6]]
        scale_to_range(one_cycle)

        return one_cycle

    def name(self):
        return 'AnalogSine'
