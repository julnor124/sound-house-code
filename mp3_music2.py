import serial
import time
import subprocess

ser = serial.Serial('/dev/cu.usbserial-140', 9600, timeout=1) # Update this line with the correct serial port (sometime it changes in Arduino and it's different for different computers).
time.sleep(2)  # Just to wait for connection to establish.

music_process = None
dog_bark_process = None 

def play_music():
    global music_process # Using global to be able to modify the music_process (and dog_bark_process) variable(s) inside the functions. 
    """Plays music using a command that opens a media player."""
    if music_process is None:  # If music is not already playing
        music_process = subprocess.Popen(["afplay", "/Users/julianordqvist/Desktop/Guess.mp3"]) # Make sure the path is correct if we're playing it from a different computer than mine.

def stop_music():
    global music_process
    """Stops the music playback."""
    if music_process is not None:  # If music is currently playing.
        music_process.terminate()  # Stop the music.
        music_process = None  # Reset the music process back to None.

def dog_barking():
    global dog_bark_process
    """Plays dog barking sound."""
    if dog_bark_process is None:  # If dog barking is not already playing.
        dog_bark_process = subprocess.Popen(["afplay", "/Users/julianordqvist/Desktop/dogs_barking_NEW.mp3"]) # Make sure the path is correct if we're playing it from a different computer than mine.

def stop_dog_barking():
    global dog_bark_process
    """Stops the dog barking playback."""
    if dog_bark_process is not None:  # If dog barking is currently playing.
        dog_bark_process.terminate()  # Stop the dog barking.
        dog_bark_process = None  # Reset the dog barking process back to None.

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8', errors='ignore').rstrip() # Reading the data sent from arduino and converting it from bytes to a string.
        print(f"Received from Arduino: {line}")  # Just for debug.
        
        # Handle music playback commands.
        if line == 'BUTTON_PRESSED':  # Connected to arduino code (See Serialprintln in arduino code).
            print("Music button pressed, starting LED and music")
            play_music()
        elif line == 'StopMusic':  # Connected to arduino code.
            print("Music button released, stopping music")
            stop_music()

        # Handle dog barking commands.
        elif line == 'DOG_BARK_BUTTON_PRESSED':  # Connected to arduino code (See Serialprintln in arduino code).
            print("Dog barking button pressed, playing barking sound")
            dog_barking()
        elif line == 'StopBarking':  # Connected to arduino code.
            print("Dog barking button released, stopping barking sound")
            stop_dog_barking()  # Stop the dog barking.
