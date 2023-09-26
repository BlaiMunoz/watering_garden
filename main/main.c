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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "dht22.h"
#include "moisture.h"
#include "wifi.h"

#define MAIN "MAIN"

void app_main(void)
{
    // Initialize the FreeRTOS logger
    esp_log_level_set("*", ESP_LOG_INFO); // Set log level to INFO for all components

    ESP_LOGI(MAIN, "Salute!\n");

    // dht22_init();
    // moisture_init();
    wifi_init();

    while (true) // Exit the loop after 2 iterations
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 10 seconds
    }

    // dht22_pause();
}
