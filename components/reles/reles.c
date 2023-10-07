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

#include "reles.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// =============================================================================
// Defines
// =============================================================================
#define RELES_TAG                                            "RELES"

#define QUEUE_LENGTH 10
#define TASK_STACK_SIZE 1024 
#define NUM_GPIO_PINS       6
#define GPIO_OUTPUT_PIN_1  16
#define GPIO_OUTPUT_PIN_2  17
#define GPIO_OUTPUT_PIN_3  5
#define GPIO_OUTPUT_PIN_4  15
#define GPIO_OUTPUT_PIN_5  2
#define GPIO_OUTPUT_PIN_6  4

#define SECONDS_IN_ONE_DAY 86400

typedef struct {
    gpio_num_t gpio_number;   // GPIO number
    uint32_t wait_ms;         // Wait time in milliseconds
} GpioSettings;

// =============================================================================
// Private Variables
// =============================================================================
static TaskHandle_t gpio_queue_thread_handle;
static TaskHandle_t gpio_watering_handle;

static GpioSettings gpio_settings[NUM_GPIO_PINS] = {
    {GPIO_OUTPUT_PIN_1, 1000},
    {GPIO_OUTPUT_PIN_2, 1000},
    {GPIO_OUTPUT_PIN_3, 1000},
    {GPIO_OUTPUT_PIN_4, 1000},
    {GPIO_OUTPUT_PIN_5, 1000},
    {GPIO_OUTPUT_PIN_6, 1000},
};

static QueueHandle_t gpio_queue;

// =============================================================================
// Private Function Declarations
// =============================================================================
/**
 * @brief Task function to control GPIOs.
 *
 * This task periodically reads GpioSettings from the gpio_queue and performs GPIO control
 * operations based on the settings. It sets the GPIO pin to LOW, waits for a specified
 * interval, and then sets the GPIO pin to HIGH. This cycle repeats for each GpioSettings
 * received from the queue.
 *
 * @param pvParameters A pointer to task parameters (not used in this task).
 */
static void rele_queue_thread(void *pvParameters);

/**
 * @brief Function to schedule periodic watering operations.
 *
 * This function is responsible for scheduling periodic watering operations based on
 * a 24-hour interval.
 *
 * @param pvParameters A pointer to task parameters (not used in this function).
 */
static void rele_set_watering(void *pvParameters);

/**
 * @brief Function to configure GPIO pins.
 *
 * It sets the GPIO pins as OUTPUT and disables interrupts.
 */
static void configure_gpios();

/**
 * @brief Function to configure the GPIO control threads.
 *
 * This function creates and starts the GPIO control threads
 */
static void configure_threads();

/**
 * @brief Function to configure the queue for GpioSettings.
 *
 * This function creates a queue to receive GpioSettings structs and checks for successful
 * creation. If the queue creation fails, it logs an error message.
 */
static void configure_queue();

// =============================================================================
// Definitions of Public Functions
// =============================================================================
void reles_init() {
    configure_queue();
    configure_gpios();
    configure_threads();
}


void reles_deinit(QueueHandle_t gpio_queue) {
    if (gpio_queue_thread_handle != NULL) {
        vTaskDelete(gpio_queue_thread_handle);
        gpio_queue_thread_handle = NULL;
    }

    // Delete the queue if it's not NULL
    if (gpio_queue != NULL) {
        vQueueDelete(gpio_queue);
        gpio_queue = NULL;
    }
}

void reles_add_watering(int slot) {
    // Check if the queue is not full, and add GpioSettings if possible
    if (xQueueSend(gpio_queue, &slot, 0) == pdPASS) {
        // Print a message indicating a parameter has been added to the queue
        ESP_LOGI(RELES_TAG, "Parameter %d has been added to the queue", slot);

        // Get the number of items in the queue
        UBaseType_t queue_count = uxQueueMessagesWaiting(gpio_queue);
        ESP_LOGI(RELES_TAG, "Number of parameters in the queue: %d/%d", queue_count, QUEUE_LENGTH);
    } else {
        ESP_LOGE(RELES_TAG, "Failed to add parameter to the queue (queue is full)");
        // Handle the error condition here if needed
    }
}
// =============================================================================
// Definitions of Private Functions
// =============================================================================
static void rele_queue_thread(void *pvParameters) {
    int gpio_number;

    while (1) {
        // Check the queue for incoming GpioSettings
        if (xQueueReceive(gpio_queue, &gpio_number, portMAX_DELAY) == pdTRUE) {
            // Set GPIO LOW
            gpio_set_level(gpio_settings[gpio_number].gpio_number, 0);

            // Wait for the configured interval duration for this GPIO
            vTaskDelay(gpio_settings[gpio_number].wait_ms / portTICK_PERIOD_MS);

            // Set GPIO HIGH during the interval
            gpio_set_level(gpio_settings[gpio_number].gpio_number, 1);
        } else {
            // Handle queue receive error if needed
        }
    }
}

static void rele_set_watering(void *pvParameters) {
    while (1) {
        for(int i = 0; i < NUM_GPIO_PINS; i++) {
            reles_add_watering(i);
        }
        vTaskDelay((SECONDS_IN_ONE_DAY * 1000) / portTICK_PERIOD_MS);
    }
}

// Function to configure the GPIOs
static void configure_gpios() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;

    for (int i = 0; i < NUM_GPIO_PINS; i++) {
        io_conf.pin_bit_mask = (1ULL << gpio_settings[i].gpio_number);
        io_conf.pull_down_en = 0;
        io_conf.pull_up_en = 0;
        gpio_config(&io_conf);
        // Set the GPIO to 1 (HIGH) after configuring
        gpio_set_level(gpio_settings[i].gpio_number, 1);
    }
}

// Function to configure the GPIO control task
static void configure_threads() {
    xTaskCreate(rele_queue_thread, "rele_queue_thread", TASK_STACK_SIZE, NULL, 5, &gpio_queue_thread_handle);
    xTaskCreate(rele_set_watering, "rele_watering_thread", 2048, NULL, 5, &gpio_watering_handle);
}

// Function to configure the queue
static void configure_queue() {
    // Create a queue to receive int
    gpio_queue = xQueueCreate(QUEUE_LENGTH, sizeof(int));

    if (gpio_queue == NULL) {
        ESP_LOGE(RELES_TAG, "Failed to create the GPIO queue");
    }
}
