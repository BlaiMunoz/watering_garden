/*
 * Project: Self watering ESP32 garden
 * Description: This header file is part of an ESP32-based Plant Watering System. The system is designed
 * to monitor air temperature, humidity, and soil moisture levels for up to six plants
 * and autonomously activate water pumps to water these plants as needed.
 *
 * Copyright (C) 2023 Blai Munoz Arnal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MOISTURE_H
#define MOISTURE_H

/**
 * @brief Initialize the Capacitive Soil Moisture Sensor.
 *
 * This function initializes the hardware and any necessary software components
 * for the Capacitive Soil Moisture Sensor. It may include tasks like configuring
 * the GPIO pins, setting up communication interfaces, or initializing sensor-specific
 * features. After initialization, the sensor is ready for use.
 */
void moisture_init();

/**
 * @brief Deinitialize the Capacitive Soil Moisture Sensor.
 *
 * This function deinitializes the Capacitive Soil Moisture Sensor, releasing any
 * allocated resources or shutting down any hardware components that were initialized
 * during the sensor's initialization. It should be called when the sensor is no longer
 * needed to clean up resources and prevent resource leaks.
 */
void moisture_deinit();

#endif /* MOISTURE_H */
