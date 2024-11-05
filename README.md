# Arduino LoRaComm
LoRaComm aims to allow easily creating pairs of portable, encrypted, point-to-point LoRa communicators.

The project is developed and tested on Arduino Uno R3's and was created as part of a university project. As such, many technical choices (language, GUI framework, etc.) do not reflect my usual preferences.

# Hardware Guide
### Required (per-device; multiply by 2)
- 1x Arduino Uno R3 or similar board (needs 3.3v out for LoRa modules; 17 digital pins needed for full )
    - Needs 3.3V out for LoRa module
    - 17 digital pins are needed for a full portable build; only 2 are needed for a minimal docked build
    - Analog pins can be repurposed as digital
- 1x REYAX RYLR998 UART LoRa modules
- A method of powering the devices (battery for portable use OR USB serial for docked/web GUI use)
- Connection medium (breadboard/jumper wires/etc.)
### Recommended (per-device; multiply by 2)
- 1x 1602 LCD + 2.2kΩ resistor (for contrast control)
    - Used to display in-progress and received messages
    - Can be used without by docking to web GUI
- 1x 4x4 digital matrix keypad
    - Used as portable input
    - Can be used without by docking to web GUI
- 1x LED + relevant resistor (330Ω used in testing)
    - Used as notification LED
- 1x USB UART adapter
    - Used for easy programming of LoRa modules
    - Can be done from Arduino directly if needed (just somewhat annoying and harder to debug)
    - See [here](https://www.hackster.io/mdraber/how-to-use-rylr998-lora-module-with-arduino-020ac4) for an excellent tutorial on programming and understanding basic usage of these LoRa modules
- A friend to use the devices with
### Fritzing Diagram
Coming soon

### Keypad Matrix Legend
Coming soon

### LoRa Module Programming
All settings for the RYLR998 LoRa modules are left at factor defaults except for:
- Network ID
    - Set to anything, as long as it matches between your two modules
    - Configure with `AT+NETWORKID=<n>`
- Address
    - One module should use `1` as the address and the other should use `2`
    - Configure with `AT+ADDRESS=<n>`

# Software Guide
### Arduino Boards
#### Dependencies
See `deps.txt` for any dependencies not included in Arduino IDE.
#### Setup
Simply create a `1loracomm` folder in your Arduino project directory and download all of the provided *.ino files from this repository.

> [!WARNING]
>The 3DES encryption/decryption key (`desKey`) provided in `1loracomm.ino` is meant to be a **placeholder** and **must** be changed before flashing.
>Since the placeholder key is publicly visible in this repository, it functionally provides no security.

Once you've modified `desKey`, flash both Arduino boards with the same code. Target LoRa addresses do not need to be specified in the code, as they are determined dynamically at runtime based on the address of the local LoRa module.

### Web GUI
#### Dependencies
- Node-RED
    - Palettes
        - `node-red-dashboard`
        - `node-red-node-serialport`
#### Setup
The web GUI is a very simple application built in Node-RED. The `flows.json` containing the GUI is included in this repository. Import it into your Node-RED editor and change the physical serial interface assigned to the serial port to match your environment.

# Demonstration of Physical Build
Coming soon

# Research documentation
To be published upon completion of university course
