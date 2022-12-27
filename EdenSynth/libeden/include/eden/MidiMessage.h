#pragma once
///
/// \author Jan Wilczek
/// \date 27.08.2018
///

namespace eden {
/// <summary>
/// Single MIDI message.
/// </summary>
class MidiMessage {
 public:
  /// <summary>
  /// Type of the message.
  /// </summary>
  enum class MidiMessageType {
    NoteOn,
    NoteOff,
    Aftertouch,
    ControlChange,
    PitchBendChange
  };

  /// <summary>
  /// Converts MIDI note number to frequency expressed in Hz.
  /// </summary>
  /// <param name="midiNoteNumber"></param>
  /// <param name="frequencyOfA"></param>
  /// <returns>calculated frequency in Hz</returns>
  static float getMidiNoteInHertz(int midiNoteNumber,
                                  const float frequencyOfA = 440.0) noexcept;

  MidiMessage() = default;

  /// <summary>
  /// Constructs a MIDI message on specified channel and of specified type.
  /// </summary>
  /// <param name="channel"></param>
  /// <param name="messageType"></param>
  MidiMessage(int channel, MidiMessageType messageType);

  /// <returns>currently set channel this message concerns</returns>
  int getChannel() const noexcept;

  /// <summary>
  /// Sets channel this message concerns.
  /// </summary>
  /// <param name="channel"></param>
  void setChannel(int channel);

  /// <returns>currently set message type</returns>
  MidiMessageType getType() const;

  /// <summary>
  /// Sets the type of this message.
  /// </summary>
  /// <param name="type"></param>
  void setType(MidiMessageType type);

  /// <returns>current note number if it is a message concerning a note (note
  /// on, note off etc.), -1 otherwise</returns>
  int getNoteNumber() const;

  /// <summary>
  /// Sets note number. Should be used only in messages concerning notes (note
  /// on, note off etc.).
  /// </summary>
  /// <param name="noteNumber"></param>
  void setNoteNumber(int noteNumber);

  /// <returns>Currently set velocity (strength of key hit)</returns>
  float getVelocity() const;

  /// <summary>
  /// Sets new velocity (key hit strength).
  /// </summary>
  /// <param name="velocity"></param>
  void setVelocity(float velocity);

  int getPitchWheelPosition();

  void setPitchWheelPosition(int position);

 private:
  MidiMessageType _type;
  int _channel = 0;
  int _noteNumber = -1;
  float _velocity = 0;
  int _pitchWheelPosition = 0;
};
}  // namespace eden