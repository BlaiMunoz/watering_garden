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

#ifndef DHT22_H
#define DHT22_H

// =============================================================================
// Imports
// =============================================================================


// =============================================================================
// Defines
// =============================================================================


// =============================================================================
// Public Variables
// =============================================================================


// =============================================================================
// Public Function Declarations
// =============================================================================

/**
 * @brief Initialize the DHT22 sensor.
 *
 * This function initializes the DHT22 sensor, preparing it for data reading.
 * It should be called once during the setup phase of your application.
 */
void dht22_init();

/**
 * @brief Deinitialize the DHT22 sensor.
 *
 * This function deinitializes the DHT22 sensor, releasing any allocated resources
 * and preparing it for shutdown. It should be called when the sensor is no longer
 * needed or during the cleanup phase of your application.
 */
void dht22_deinit();

#endif /* DHT22_H */
