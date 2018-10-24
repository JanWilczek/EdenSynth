#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "24.10.2018"

from WaveformGenerator.Generator import Generator
import numpy as np
import scipy.signal as signal


class AnalogSawtoothGenerator(Generator):
    def generate(self, length):
        freq = 1000
        fs = freq * length
        nb_repetitions = 4

        delta = freq / fs

        phase = np.arange(0, nb_repetitions, delta)
        mod_phase = (phase + delta) % 1              # modulo counter
        bphase = 2 * mod_phase - 1                   # bipolar modulo counter - crude sawtooth

        sq = np.power(bphase, 2)

        a = [2]
        b = [1, 0, -1]

        zi = signal.lfilter_zi(b, a)
        z, _ = signal.lfilter(b, a, sq, zi=zi * sq[0])

        c = fs / (4 * freq * (1 - delta))            # scaling factor
        scaled_output = c * z

        after_cycle = np.where(scaled_output[length:] < scaled_output[0])   # find cycle completion
        one_cycle = scaled_output[0:length + after_cycle[0][0]]

        return one_cycle

    def name(self):
        return 'AnalogSawtoothRampUp'
