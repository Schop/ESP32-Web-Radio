# ESP32 Web Radio - PlatformIO Project

This project has been converted from Arduino IDE format to PlatformIO format for better development experience and dependency management.

## Project Structure

```
ESPwebRadio/
├── platformio.ini          # PlatformIO configuration
├── src/                    # Source files (.cpp)
│   ├── main.cpp           # Main program (was ESPwebRadio.ino)
│   ├── audio.cpp          # Audio streaming and MP3 decoding
│   ├── rotary.cpp         # Rotary encoder handling
│   ├── lcdisplay.cpp      # LCD display management
│   ├── senderlist.cpp     # Station list initialization
│   ├── senderconfig.cpp   # Web interface for station configuration
│   └── wlanconfig.cpp     # WiFi configuration system
├── include/               # Header files (.h)
│   ├── senderconf.h       # HTML template for station config
│   └── wlanconf.h         # HTML template for WiFi config
└── README.md              # This file
```

## Hardware Requirements

- ESP32 development board
- I2S DAC or amplifier for audio output
- 16x2 I2C LCD display (address 0x27)
- Rotary encoder with push button
- Speakers or headphones

## Pin Connections

### Rotary Encoder
- Pin A: GPIO 33
- Pin B: GPIO 32
- Button: GPIO 34
- VCC: 3.3V
- GND: GND

### LCD Display (I2C)
- SDA: GPIO 21
- SCL: GPIO 22
- VCC: 3.3V or 5V
- GND: GND

### Audio Output (I2S)
- Uses internal DAC for audio output
- Connect amplifier/speakers to DAC pins

## Library Dependencies

The following libraries are automatically installed by PlatformIO:

- **ESP8266Audio**: For MP3 streaming and decoding
- **LiquidCrystal_I2C**: For LCD display control
- **Ai Esp32 Rotary Encoder**: For rotary encoder handling
- **ESPAsyncWebServer-esphome**: For web interface

## Build and Upload

1. Install PlatformIO in VS Code or use PlatformIO CLI
2. Open this project folder in PlatformIO
3. Build the project: `pio run`
4. Upload to ESP32: `pio run --target upload`
5. Monitor serial output: `pio device monitor`

## Configuration

### WiFi Setup
1. On first boot, the device creates a WiFi access point "WEBRADIO"
2. Connect to this network and navigate to http://192.168.4.1
3. Enter your WiFi credentials
4. Device will reboot and connect to your network

### Station Configuration
1. Find the device's IP address from serial monitor or router
2. Navigate to the device's IP in a web browser
3. Edit station names and URLs
4. Click "alles speichern" to save changes

## Features

- 20 configurable internet radio stations
- Rotary encoder for station selection
- LCD display showing current station
- Web interface for configuration
- Persistent storage of settings
- Automatic WiFi reconnection
- Metadata display for song information

## Usage

- **Rotate encoder**: Browse through stations
- **Press encoder**: Select and play current station
- **Web interface**: Configure stations and WiFi settings
- **LCD display**: Shows current station and playing indicator

## Default Stations

The project comes with 20 pre-configured German radio stations including NDR, WDR, Bayern, and others. These can be modified through the web interface.

## Troubleshooting

- If compilation fails, ensure all libraries are properly installed
- Check serial monitor for connection and error messages
- Verify pin connections match the configuration
- Make sure ESP32 board is properly selected in platformio.ini