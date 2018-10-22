from scipy.io.wavfile import write


class WaveWriter:
    @staticmethod
    def save_wave_file(filename, samples, fs):
        if not filename.endswith('.wav'):
            filename = filename + '.wav'

        write(filename, fs, samples)
