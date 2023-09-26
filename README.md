# ESP32 Plant Watering System

## Repository Overview

This repository contains the source code and documentation for an ESP32-based Plant Watering System. The system is designed to monitor air temperature, humidity, and soil moisture levels for up to six plants and autonomously activate water pumps to water these plants as needed.

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Getting Started](#getting-started)
  - [Setting up the Development Environment](#setting-up-the-development-environment)
  - [Configuring the ESP32](#configuring-the-esp32)
  - [Building the Firmware](#building-the-firmware)
  - [Flashing the Firmware](#flashing-the-firmware)
- [Usage](#usage)
  - [Monitoring Sensors](#monitoring-sensors)
  - [Plant Watering](#plant-watering)
- [Contributing](#contributing)
- [License](#license)

## Features

- Measures air temperature and humidity using integrated sensors.
- Monitors soil moisture for up to six individual plants.
- Autonomous plant watering based on soil moisture levels.
- Real-time data monitoring through a web interface.
- Customizable watering thresholds and schedules.
- Energy-efficient design for prolonged battery life.

## Hardware Requirements

To build and use this system, you will need the following hardware components:

- ESP32 development board (e.g., ESP32 DevKitC)
- DHT22 or similar temperature and humidity sensor
- Soil moisture sensors (up to 6)
- Water pumps or solenoid valves (up to 6)
- Water reservoir and tubing
- Power supply (battery or mains power)
- Jumper wires
- Breadboard or custom PCB (if not using a development board)

## Software Requirements

To develop and run this firmware, you'll need the following software tools:

- Espressif IoT Development Framework (ESP-IDF)
- CMake
- Python (for ESP-IDF tools)
- A code editor or integrated development environment (IDE) of your choice (e.g., Visual Studio Code, PlatformIO)

## Getting Started

Follow these steps to set up, build, and flash the Plant Watering System firmware to your ESP32 device.

### Setting up the Development Environment

1. Install the required software tools:
   - Install ESP-IDF by following the [official installation guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html).

2. Clone this repository to your local machine:

   ```shell
   git clone https://github.com/your-username/esp32-plant-watering-system.git
   ```

3. Navigate to the project directory:

   ```shell
   cd esp32-plant-watering-system
   ```
4. Seeting up the ennvironment
   -  Before building, flashing or doing any other action the user must execute the following command once:
   ```shell
   . $HOME/esp/esp-idf/export.sh
   ```

### Configuring the ESP32

1. Configure your ESP32 settings using the `idf.py menuconfig` command. This will allow you to set up Wi-Fi credentials, sensor pins, watering thresholds, and other parameters.

   ```shell
   idf.py menuconfig
   ```

2. Save your configuration settings.

### Building the Firmware

1. Build the firmware using the following command:

   ```shell
   idf.py build
   ```

### Flashing the Firmware

1. Connect your ESP32 to your computer using a USB cable.

2. Flash the firmware to your ESP32:

   ```shell
   idf.py -p PORT flash
   ```

   Replace `PORT` with the appropriate serial port for your ESP32 device.

## Usage

### Monitoring Sensors

Once the firmware is successfully flashed onto your ESP32, it will start monitoring the air temperature, humidity, and soil moisture levels automatically. You can access the real-time sensor data through a web interface.

1. Connect to the ESP32's Wi-Fi network (configured during `menuconfig`).

2. Open a web browser and navigate to the ESP32's IP address. You can find the IP address in the serial monitor or by checking your router's connected devices list.

3. You will be able to view the sensor readings on the web interface.

### Plant Watering

The system will autonomously activate the water pumps or solenoid valves based on the soil moisture thresholds you set in the configuration. It will water the plants when the soil moisture falls below the defined threshold.

## Contributing

Contributions to this project are welcome! If you have ideas for improvements or new features, please open an issue or submit a pull request. Be sure to follow the project's coding and contribution guidelines.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

Thank you for using the ESP32 Plant Watering System! If you have any questions or encounter issues, please feel free to open an issue in this repository for assistance.
