# Spotify Macrokeyboard

![The Project](https://i.imgur.com/VzZgObq.jpg)

This project is a macro keyboard that uses the Spotify API to control spotify music, it uses an ESP32 to make requests to my own personal server which then makes requests on the ESP32's behalf to speed up the process.

## Installation

To install this project you will need to have some sort of software to upload the code to the ESP32.

For simplicity purposes you can just use the Arduino IDE.

Clone the project using:

```bash
$ git clone https://github.com/Leg3ndary/SpotifyMacroBoard.git
```

You also need to install a few other libaries that can be found online or in the Arduino IDE library manager.

Remember that you need to fill out a similar file to the `SampleCredentials.h` header file so that you can communicate with whatever server you choose to run.

You also need to use a 2.4 GHz network since the ESP32 doesn't support 5 GHz networks.

## Usage

You will also need to print out and solder the PCB that is included within the project files.

Once you have all the parts, you can solder the PCB and upload all of the program.

Finally, you can print out the keycaps or use regular ones, you also need to get mechanical switches of whatever kind you prefer.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
