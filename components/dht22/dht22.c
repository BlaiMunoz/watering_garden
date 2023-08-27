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

#include <stdio.h>
#include <unistd.h>

#include "esp_log.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht22.h"


// =============================================================================
// Defines
// =============================================================================

#define DHT22_TAG "DHT22"
#define GPIO_DHT22 22
#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2
#define MAXdhtData 5	// to complete 40 = 5*8 Bits

// =============================================================================
// Global Variables
// =============================================================================
static TaskHandle_t dht22_task_handle = NULL;
static bool dht22_task_running = false;
static int DHTgpio = 4;
static float humidity = 0.;
static float temperature = 0.;


// =============================================================================
// Private Function Declarations
// =============================================================================
/**
 * @brief Task function for reading DHT22 sensor data.
 *
 * This function is responsible for periodically reading data from a DHT22 sensor.
 * It should be used as a FreeRTOS task.
 *
 * @param[in] pvParameters Pointer to task parameters ().
 */
static void dht22_task(void *pvParameters);

/**
 * @brief Set the DHT GPIO pin.
 *
 * This static function sets the GPIO pin used to communicate with the DHT sensor.
 *
 * @param gpio The GPIO pin to set for DHT communication.
 */
static void set_dht_gpio(int gpio);

/**
 * @brief Handle DHT sensor communication errors.
 *
 * This static function handles errors that may occur during DHT sensor communication.
 *
 * @param response The response code indicating the communication status.
 */
static void error_handler(int response);

/**
 * @brief Read data from the DHT sensor.
 *
 * This static function reads data from the DHT sensor and returns an integer response code.
 *
 * @return An integer response code indicating the success or failure of the data read.
 */
static int read_dht();

/**
 * @brief Get the signal level on a GPIO pin.
 *
 * This static function measures the signal level on a GPIO pin for a specified timeout and state.
 *
 * @param usTimeOut The timeout value in microseconds.
 * @param state The expected state (high or low) for the signal level.
 * @return An integer value representing the signal level.
 */
static int get_signal_level(int usTimeOut, bool state);


// =============================================================================
// Definitions of Public Functions
// =============================================================================

void dht22_init() {
    if (!dht22_task_running){
        ESP_LOGI(DHT22_TAG, "Start dht22 sensor!\n");
        set_dht_gpio(GPIO_DHT22);
        dht22_task_running = true;
        // Create and start the DHT22 task
        xTaskCreate(dht22_task, "dht22_task", 4096, NULL, 5, &dht22_task_handle);
    }
}

void dht22_deinit() {
    if (dht22_task_running) {
        ESP_LOGI(DHT22_TAG, "Stop dht22 sensor!\n");
        // Stop the DHT22 task
        vTaskDelete(dht22_task_handle);
        dht22_task_handle = NULL;
        dht22_task_running = false;
    }
}


// =============================================================================
// Definitions of Private Functions
// =============================================================================

static void dht22_task(void *pvParameters) {
    (void)pvParameters;
    ESP_LOGI(DHT22_TAG, "dht22 taskcreated: %d!\n", dht22_task_running);
    
    while (dht22_task_running) {
        // Read DHT22 sensor data
        ESP_LOGI(DHT22_TAG, "DHT Sensor Readings\n" );
		int ret = read_dht();
		
		error_handler(ret);

		ESP_LOGI(DHT22_TAG, "Humidity %.2f %%\n", humidity);
		ESP_LOGI(DHT22_TAG, "Temperature %.2f degC\n\n", temperature);

        // Delay for 4 seconds
        vTaskDelay(4000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

static void set_dht_gpio( int gpio )
{
	DHTgpio = gpio;
}

static void error_handler(int response)
{
	switch(response) {
		case DHT_TIMEOUT_ERROR :
			ESP_LOGE(DHT22_TAG, "Sensor Timeout\n" );
			break;
		case DHT_CHECKSUM_ERROR:
			ESP_LOGE(DHT22_TAG, "CheckSum error\n" );
			break;
		case DHT_OK:
			break;
		default :
			ESP_LOGE(DHT22_TAG, "Unknown error\n" );
	}
}

int read_dht()
{
    int uSec = 0;

    uint8_t dhtData[MAXdhtData];
    uint8_t byteInx = 0;
    uint8_t bitInx = 7;

	for (int k = 0; k<MAXdhtData; k++) 
		dhtData[k] = 0;

	// == Send start signal to DHT sensor ===========
	gpio_set_direction( DHTgpio, GPIO_MODE_INPUT );
	usleep( 10 );			

	gpio_set_direction( DHTgpio, GPIO_MODE_OUTPUT );

	// pull down for 1,1 ms for a smooth and nice wake up 
	gpio_set_level( DHTgpio, 0 );
	usleep( 1100 );			

	// pull up for 25 us for a gentile asking for data
	gpio_set_level( DHTgpio, 1 );
	usleep( 55 );

	gpio_set_direction( DHTgpio, GPIO_MODE_INPUT );		// change to input mode
  
	// == DHT will keep the line low for 80 us and then high for 80us ====

	uSec = get_signal_level( 85, 0 );
//	ESP_LOGI(DHT22_TAG, "Response = %d", uSec );
	if( uSec<0 ) return DHT_TIMEOUT_ERROR; 

	// -- 80us up ------------------------

	uSec = get_signal_level( 85, 1 );
//	ESP_LOGI(DHT22_TAG, "Response = %d", uSec );
	if( uSec<0 ) return DHT_TIMEOUT_ERROR;

	// == No errors, read the 40 data bits ================
  
	for( int k = 0; k < 40; k++ ) {

		// -- starts new data transmission with >50us low signal

		uSec = get_signal_level( 56, 0 );
		if( uSec<0 ) return DHT_TIMEOUT_ERROR;

		// -- check to see if after >70us rx data is a 0 or a 1

		uSec = get_signal_level( 75, 1 );
		if( uSec<0 ) return DHT_TIMEOUT_ERROR;

		// add the current read to the output data
		// since all dhtData array where set to 0 at the start, 
		// only look for "1" (>28us us)
	
		if (uSec > 40) {
			dhtData[ byteInx ] |= (1 << bitInx);
			}
	
		// index to next byte

		if (bitInx == 0) { bitInx = 7; ++byteInx; }
		else bitInx--;
	}

	// == get humidity from Data[0] and Data[1] ==========================

	humidity = dhtData[0];
	humidity *= 0x100;					// >> 8
	humidity += dhtData[1];
	humidity /= 10;						// get the decimal

	// == get temp from Data[2] and Data[3]
	
	temperature = dhtData[2] & 0x7F;	
	temperature *= 0x100;				// >> 8
	temperature += dhtData[3];
	temperature /= 10;

	if( dhtData[2] & 0x80 ) 			// negative temp, brrr it's freezing
		temperature *= -1;


	// == verify if checksum is ok ===========================================
	// Checksum is the sum of Data 8 bits masked out 0xFF
	
	if (dhtData[4] == ((dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3]) & 0xFF))
	{
		return DHT_OK;
	}else
	{
		return DHT_CHECKSUM_ERROR;
	}
}

static int get_signal_level( int usTimeOut, bool state )
{

	int uSec = 0;
	while( gpio_get_level(DHTgpio)==state ) {

		if( uSec > usTimeOut ) 
			return -1;
		++uSec;
		usleep(1);		// uSec delay
	}
	return uSec;
}