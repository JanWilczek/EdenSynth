from WaveformGenerator.Generator import Generator
import numpy as np


class TriangleGenerator(Generator):
    def generate(self, length):
        t = np.arange(0, 1, 1 / length)
        output = np.zeros(length)

        first_ramp = t <= 0.25
        output[first_ramp] = 4 * t[first_ramp]

        second_ramp = (t > 0.25) & (t <= 0.75)
        output[second_ramp] = -4 * t[second_ramp] + 2

        third_ramp = t > 0.75
        output[third_ramp] = 4 * t[third_ramp] - 4

        return output

