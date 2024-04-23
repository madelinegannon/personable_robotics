import mido

# Installation instructions for mido: https://mido.readthedocs.io/en/latest/installing.html
# python -m pip install mido[ports-rtmidi]

def midi_to_string(midi_msg):
    print(f"Incoming MIDI Message: {midi_msg} ")
    if "type" in midi_msg.dict():
        print(f"\tType: {midi_msg.dict()['type']}")
    if "channel" in midi_msg.dict():
        print(f"\tChannel: {midi_msg.dict()['channel']}")
    if "note" in midi_msg.dict():
        print(f"\tNote: {midi_msg.dict()['note']}")
    if "velocity" in midi_msg.dict():
        print(f"\tVelocity: {midi_msg.dict()['velocity']}")
    if "value" in midi_msg.dict():
        print(f"\tValue: {midi_msg.dict()['value']}")
    if "time" in midi_msg.dict():
        print(f"\tTime: {midi_msg.dict()['time']}")
    if "control" in midi_msg.dict():
        print(f"\tControl: {midi_msg.dict()['control']}")
    if "pitch" in midi_msg.dict():
        print(f"\tPitch: {midi_msg.dict()['pitch']}")
    print()

# Main loop to listen for MIDI messages
with mido.open_input() as inport:
    print("Listening for MIDI messages...")
    for msg in inport:
        midi_to_string(msg)