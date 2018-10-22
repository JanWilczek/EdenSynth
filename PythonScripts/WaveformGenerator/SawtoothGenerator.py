from WaveformGenerator.Generator import Generator
import numpy as np


class SawtoothGenerator(Generator):
    def generate(self, length):
        t = np.arange(0, 1, 1 / length)

        output = 2 * t - 1

        return output