from WaveformGenerator.WaveWriter import WaveWriter
from WaveformGenerator.SineGenerator import SineGenerator
from WaveformGenerator.TriangleGenerator import TriangleGenerator
from WaveformGenerator.SquareGenerator import SquareGenerator
from WaveformGenerator.SawtoothGenerator import SawtoothGenerator


def generate_and_write(generator, name, length):
    samples = generator.generate(length)
    WaveWriter.save_wave_file(name, samples, length)


if __name__ == "__main__":
    length = 48000

    generate_and_write(SineGenerator(), 'Sine', 48000)
    generate_and_write(TriangleGenerator(), 'Triangle', 48000)
    generate_and_write(SquareGenerator(), 'Square', 48000)
    generate_and_write(SawtoothGenerator(), 'SawtoothRampUp', 48000)
