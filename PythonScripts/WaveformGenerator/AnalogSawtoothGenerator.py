#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "24.10.2018"

import numpy as np
import scipy.signal as signal
from WaveformGenerator.Generator import Generator
from WaveformGenerator.utils import limit


class AnalogSawtoothGenerator(Generator):
    def generate(self, length):
        return self.generate_integrative(length)

    def generate_integrative(self, length):
        freq = 1
        fs = freq * length
        nb_repetitions = 4

        delta = freq / fs

        gen_phase = np.arange(0, nb_repetitions, delta)
        mod_phase = gen_phase % 1                       # modulo counter
        bphase = 2 * mod_phase - 1                      # bipolar modulo counter - crude sawtooth

        sq = np.power(bphase, 2)

        a = [2]
        b = [1, 0, -1]

        zi = signal.lfilter_zi(b, a)
        z, _ = signal.lfilter(b, a, sq, zi=zi * sq[0])

        c = fs / (4 * freq * (1 - delta))               # scaling factor
        scaled_output = c * z

        after_cycle = np.where(scaled_output[length:] < scaled_output[0])   # find cycle completion
        one_cycle = scaled_output[0:length + after_cycle[0][0]]

        return one_cycle

    def generate_iterative(self, length):
        freq = 1
        sample_rate = length
        delta = freq / sample_rate

        c = sample_rate / (4 * freq * (1 - freq/sample_rate))

        t = np.arange(0, 1, 1 / sample_rate)
        phase_v = 2 * np.pi * t
        samples = np.zeros(len(phase_v))
        squared = np.zeros(len(phase_v))

        phase = 0

        for i in range(0, len(phase_v)):
            phase = np.mod(phase + delta, 1)
            bphase = 2 * phase - 1

            sq = bphase ** 2
            squared[i] = sq

            dsq = (sq - squared[i - 2]) / 2

            out = c * dsq
            samples[i] = out

        limit(samples)

        return samples

    def name(self):
        return 'AnalogSawtoothRampUp'
