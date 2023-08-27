/*
 * Project: Self watering ESP32 garden
 * Description: This source file is part of an ESP32-based Plant Watering System. The system is designed
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

// =============================================================================
// Imports
// =============================================================================

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


// =============================================================================
// Defines
// =============================================================================

#define MOISTURE_TAG "MOISTURE SENSOR"
#define SENSOR_PIN 21


// =============================================================================
// Private Variables
// =============================================================================

static TaskHandle_t moisture_task_handle = NULL;
static bool moisture_task_running = false;


// =============================================================================
// Private Function Declarations
// =============================================================================
/**
 * @brief Task function to read data from the Capacitive Soil Moisture Sensor.
 *
 * This task periodically reads data from the Capacitive Soil Moisture Sensor connected
 * to a specific GPIO pin (defined as SENSOR_PIN) on the ESP32. It prints the sensor value
 * and then waits for a specified interval before taking another reading.
 *
 * @param pvParameters A pointer to task parameters (not used in this task).
 */
static void sensor_task(void *pvParameters);


// =============================================================================
// Definitions of Public Functions
// =============================================================================

void moisture_init() {
    if (!moisture_task_running){
		ESP_LOGI(MOISTURE_TAG, "Init dht22 sensor!\n");
		// Initialize GPIO for the sensor pin
		gpio_set_direction(SENSOR_PIN, GPIO_MODE_INPUT);

		// Create the sensor task
		xTaskCreate(sensor_task, "sensor_task", 2048, NULL, 5, &moisture_task_handle);
	}
}

void moisture_deinit() {
    if (moisture_task_running) {
        ESP_LOGI(MOISTURE_TAG, "Deinit dht22 sensor!\n");
        // Stop the DHT22 task
        vTaskDelete(moisture_task_handle);
        moisture_task_handle = NULL;
        moisture_task_running = false;
    }
}

// =============================================================================
// Definitions of Private Functions
// =============================================================================

static void sensor_task(void *pvParameters) {
    while (1) {
        // Read data from the Capacitive Soil Moisture Sensor
        int sensor_value = gpio_get_level(SENSOR_PIN);

        // Print the sensor value
		if( sensor_value == 1 )
		{
        	printf("Soil is DRY\n\n");
		}else {
        	printf("Soil is WET\n\n");
		}

        // Delay for some time before the next reading (adjust as needed)
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 1 second
    }
}