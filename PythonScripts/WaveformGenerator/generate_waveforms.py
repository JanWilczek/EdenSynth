#!/usr/bin/env python
__author__ = "Jan Wilczek"
__date__ = "22.10.2018"

from WaveformGenerator.WaveWriter import WaveWriter
from WaveformGenerator.SineGenerator import SineGenerator
from WaveformGenerator.TriangleGenerator import TriangleGenerator
from WaveformGenerator.SquareGenerator import SquareGenerator
from WaveformGenerator.SawtoothGenerator import SawtoothGenerator


def get_wavetables_path():
    return ''


def generate_and_write(generator, length):
    samples = generator.generate(length)
    WaveWriter.save_wave_file(get_wavetables_path() + generator.name(), samples, length)


if __name__ == "__main__":
    table_length = 48000

    generate_and_write(SineGenerator(), table_length)
    generate_and_write(TriangleGenerator(), table_length)
    generate_and_write(SquareGenerator(), table_length)
    generate_and_write(SawtoothGenerator(), table_length)