#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "24.10.2018"

import numpy as np
import scipy.signal as signal
from WaveformGenerator.Generator import Generator
from WaveformGenerator.utils import scale_to_range, resample


class AnalogSawtoothGenerator(Generator):
    def generate(self, length):
        # constants from Valimaki's article
        fs = 44100
        freq = 2793.8
        nb_repetitions = 3

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

        resampled_output = resample(scaled_output, fs_in=round(fs / freq), fs_out=length)

        zero_crossings = np.where(np.diff(np.sign(resampled_output)))[0]
        one_cycle = resampled_output[zero_crossings[0] : zero_crossings[2]]

        scale_to_range(one_cycle)

        return one_cycle

    def name(self):
        return 'AnalogSawtoothRampUp'
