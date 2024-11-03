# ESP8266 Fan Controller with Web Interface

This project is an ESP8266-based controller for three PWM fans, which allows remote control of fan speeds through a web interface. You can set individual fan speeds (in percentage) and optionally set timers to turn off the fans automatically after a set number of hours. This setup is ideal for controlling fans or similar PWM devices.

## Features

- Control up to **3 fans** individually with speed control (0-100%)
- Set timers for each fan to automatically turn off after a specified number of hours
- Web-based interface with mobile-friendly design
- **Over-the-Air (OTA)** updates for easy firmware deployment
- Built-in **ESP8266 Wi-Fi** connectivity

## Hardware Requirements

- **ESP8266** (e.g., NodeMCU, Wemos D1 Mini)
- Three PWM-controllable fans or motors (connect to D5, D6, and D7)
- Power supply compatible with the fans

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) with **ESP8266 Board Package**
- Libraries:
  - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
  - [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

## Installation

1. **Clone or Download** this repository to your local machine.
2. **Install Required Libraries**:
   - Open Arduino IDE and go to **Library Manager** (`Sketch` > `Include Library` > `Manage Libraries...`)
   - Search and install `ESPAsyncWebServer` and `ESPAsyncTCP`.
3. **Set Up Wi-Fi Credentials**:
   - Open the Arduino sketch and enter your Wi-Fi SSID and password in the `setup` function:
     ```cpp
     const char* ssid = "YOUR_SSID";
     const char* password = "YOUR_PASSWORD";
     ```
4. **Upload Code** to your ESP8266 using Arduino IDE.
5. **Access the Web Interface**:
   - Once connected to Wi-Fi, open a web browser and navigate to the IP address shown in the Serial Monitor.
   - Control fan speeds and timers directly from the interface.

## Usage

### Web Interface

- **Fan Speed Control**: Choose from 0% to 100% for each fan individually by pressing the corresponding buttons.
- **Timer**: Set a timer to turn off all fans automatically after 1, 2, 4, or 8 hours. Set to "OFF" to disable the timer.

### API Endpoints

This project provides simple HTTP endpoints for direct API control:

1. **Set Fan Speed**: `/setfan?fan={fan}&value={speed}&hours={timer}`
   - **fan**: Fan number (1, 2, or 3)
   - **value**: Speed percentage (0-100)
   - **hours**: Timer setting in hours (0 to disable)
   - Example: `/setfan?fan=1&value=70&hours=2`

2. **Get Fan Speed**: `/getFanSpeed?fan={fan}`
   - **fan**: Fan number (1, 2, or 3)
   - Example: `/getFanSpeed?fan=1`

## Code Overview

- **Fan Control**: PWM is used to set fan speeds, with a duty cycle corresponding to the selected percentage (0-100%).
- **Timers**: Automatically turns off fans after the set duration (in hours).
- **OTA**: Over-the-Air updates are enabled for easy firmware updates without physical access.

## Future Improvements

- **Fan Status Indicator**: Show current fan speed and timer settings on the web interface.
- **Additional Sensors**: Integrate temperature or humidity sensors for automated fan control based on environmental conditions.
- **OpenHAB Integration**: Further customize for smart home integration.

## License

This project is open-source and available under the [MIT License](LICENSE).

---

Enjoy your smart fan control setup! Feel free to contribute to this project by submitting pull requests.
