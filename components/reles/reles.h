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

#ifndef RELES_H_
#define RELES_H_

#include "esp_system.h"

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
 * @brief Initialize the Reles Control System.
 *
 * This function initializes the Reles Control System. It configures GPIO pins, creates
 * a task to control GPIOs, and sets up a queue to receive GpioSettings for controlling
 * different aspects of the system.
 *
 * @note Call this function to initialize the system before using any other functionality.
 */
void reles_init();

/**
 * @brief Deinitialize the Reles Control System.
 *
 * This function deinitializes the Reles Control System. It stops the GPIO control task,
 * deletes the queue, and performs cleanup operations as needed.
 *
 * @note Call this function to gracefully shut down the system when it's no longer in use.
 */
void reles_deinit();

/**
 * @brief Add GpioSettings to the GPIO Control Queue.
 *
 * This function the slot to be watered to a queue.
 *
 * @param slot The slot to water.
 *
 * @note Use this function to schedule GPIO control operations by adding them to the queue.
 */
void reles_add_watering(int slot);

#endif /* RELES_H_ */
