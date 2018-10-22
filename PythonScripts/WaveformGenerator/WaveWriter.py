#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "22.10.2018"

from scipy.io.wavfile import write


class WaveWriter:
    @staticmethod
    def save_wave_file(filename, samples, fs):
        if not filename.endswith('.wav'):
            filename = filename + '.wav'

        write(filename, fs, samples)
