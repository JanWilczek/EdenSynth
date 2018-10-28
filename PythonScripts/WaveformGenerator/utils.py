#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "26.10.2018"

import numpy as np
from scipy.signal import resample as scipy_resample


def limit(samples):
    samples[samples > 1] = 1
    samples[samples < -1] = -1


def scale_to_range(signal):
    max_val = np.max(signal)
    min_val = np.min(signal)
    scale_factor = max(abs(max_val), abs(min_val))

    signal /= scale_factor


def resample(signal, fs_in, fs_out):
    resampled_length = int(len(signal) * fs_out / fs_in)
    return scipy_resample(signal, resampled_length)
