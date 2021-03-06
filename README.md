Scoreboard
============
## Project
This project is an arena-style scoreboard with wireless remote control.  There are 4 digits for the clock (timing) and 2 digits each for the team scores (8-digits total).  Each digit is a 5x7 array consisting of 35 WS2811 LEDs (the pixels).  Each digit is controlled by a GPIO line from an Arduino Mega 2650 (or similar). A 5x7 font ASCII character set is included. 

## Operation
The scoreboard and remote each have 3 switches.  You can operate the 3 switches locally on the scoreboard or the 3 switches on the remote.   Two switches control the score(s) and one other controls the clock timer.  Press and hold a switch to clear its function.   Press multiple switches at once to run test patterns (or create new features). 

## Circuitry
Scoreboard: The scoreboard is powered by an Arduino Mega 2650 (5VDC).  There are 8 GPIO lines used to drive the data line for each string of WS2811 LEDs with 35 LEDs per string.  The 3 momentary switches are wired to GPIO pins.  Each switch is debounced in software using a state machine.   An nRF24L01 2.4GHz radio transceiver (receiver) is wired to the SPI bus along with a GPIO for the radio's chip select CS.  A GPIO relay circuit activates a buzzer when the time expires.

Remote:  The remote is powered by a Feather 32u4 (3.3VDC).  It is wired similar to the scoreboard in that 3 switches are wired to GPIO lines and the nRF24L01 radio transceiver (transmitter) is wired to the SPI bus with a chip select CS pin.  A 1200mAh battery is connected to the VBAT+ charging connector on the Feather 32u4.  The battery is recharged when this board is connected to a USB power source. 

## Hardware 
Bill of Materials (Electronics)

### SCOREBOARD
* Arduino Mega 2650 (chosen because of its pin count and 5V I/O)
* Switches, Momentary
* nRF24L01
* Power Supply, 5V 20A
* Wire, 22AWG
* Protoboard

### REMOTE
* Adafruit Feather 32u4 Basic Proto #2771 (chosen because of its form factor, 3.3V I/O, onboard battery charger)
* Switches, Momentary
* nRF24L01 
* Battery, LiPo 1200mAh
* Protoboard

## Software
### SCOREBOARD
This code is procedural and operates using the conventional Arduino setup/loop procedure.

Setup:  The scoreboard's setup() function assigns the hardware pinouts and initializes the nRF24L01 radio as a receiver.

Loop: The scoreboard's loop() function performs these actions:
* Polling switches (debounce, press/release/hold state machine, creates a command based on what button was pressed)
* Polling radio receiver (for messages)
* Message Loop (decode)
* Timebase
* Time expired handler
* Rendering
* Fonts (5x7)

### REMOTE
Setup:  The remote's setup() function assigns the hardware pinouts and initializes the nRF24L01 radio as a transmitter.

Loop: The remote's loop() function performs these actions:
Polling switches (same operation/code as scoreboards switches above)

Encodes and sends a message via the radio transmitter based on what button was pressed.   See Protocol, next.

## Message Protocol
A minimalist message protocol sends a 1-byte command to indicate what button was pressed.  A message header containing the characters "$SB>" indicates the start, device, and direction of the message.  Optionally, a message length and checksum are added.  The receive buffer on the nRF24L01 is 32-bytes.  

## Libraries
The [FastLED](https://github.com/FastLED/FastLED "FastLED") library is used to drive the LEDs. 

The [nRF24L01](https://github.com/tmrh20/RF24/ "nRF") library is used to drive the radio transceivers.

## Files
The main project is in the folder: scoreboard

The wireless remote project is in the folder: scoreboard_controller


