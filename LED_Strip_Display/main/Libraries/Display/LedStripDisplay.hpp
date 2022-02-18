/*
	MIT License

	Copyright (c) 2022 Reza Bahrami @ https://github.com/RBahrami

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
/*
 * This library uses led_strip driver by UncleRus @ https://github.com/UncleRus/esp-idf-lib
 * For more information and how to use this library refer to README.md
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include <esp_log.h>

#include <led_strip.h>

namespace EE
{
	class LedStripDisplay
	{
	public:
		typedef rgb_t Color_t;

		/**
		 * @brief  Constructor of LedStripDisplay
		 * @note   Can be used standalone or be a base class for GFX class
		 * @param  width: The width of display
		 * @param  height: The hight of display
		 * @param  waitToBeFree: number of ticks to wait until display memory is free to be able to update display (default: portMax_DELAY)
		 */
		LedStripDisplay(int16_t width, int16_t height, TickType_t waitToBeFree = portMAX_DELAY)
		{
			_width = width;
			_height = height;
			_waitToBeFree = waitToBeFree;
			displaySemaphore = xSemaphoreCreateMutex();
		}

		/* -------------------------- Core member functions ------------------------- */
		/**
		 * @brief  Initialize Display
		 * @note   To be able to easley use this and other display classes as base class for GFX class,
		 * 		   the initialization of the display driver is done separately from construction of class
		 * @param  type: Type of LED strip
		 * @param  gpioNumber: GPIO number that LED strip is connected to
		 * @param  rmtChannel: Number of RTM channel that will be used for LED strip
		 * @param  brightness: Default brightness of display (0-100%)
		 * @param  pixelsMap_p: Pointer to pixels map array. Pass NULL to disable mapping
		 * @retval None
		 */
		void Init(led_strip_type_t type, gpio_num_t gpioNumber, rmt_channel_t rmtChannel, float brightness, const uint32_t *pixelsMap_p);

		/**
		 * @brief  Set color of the pixel
		 * @note   This function isn't thread safe
		 * @param  x: x cordinate of the pixel
		 * @param  y: y cordinate of the pixel
		 * @param  color: Color of the pixel
		 * @retval None
		 */
		void SetPixel(int16_t x, int16_t y, Color_t color);

		/**
		 * @brief  Update display (transmit buffer to display)
		 * @note   This function is thread safe
		 * @retval
		 */
		esp_err_t Update(void);

		/**
		 * @brief  function that locks access to the display buffer, use to sync resources
		 * @note   after calling this function and performing process, "EndWrite()" must be called to release the display buffer
		 * @param  ticks: number of ticks to wait to display buffer is released (from last lock)
		 * @retval ESP_OK: if lock is successful, ESP_ERR_TIMEOUT: in case of timeout occurs
		 */
		esp_err_t StartWrite(const TickType_t ticks = portMAX_DELAY);

		/**
		 * @brief  function that release the display buffer, use to sync resources
		 * @note   this function must be called after "StartWrite()" to allow further access to display buffer
		 * @retval None
		 */
		void EndWrite(void);

		/**
		 * @brief  Set brightness of display
		 * @note   Must use Update afterward
		 * @param  brightness: Brightness of display (0-100%)
		 * @retval None
		 */
		void SetBrightness(float brightness);

		/* --------------------- Color related member functions --------------------- */
		/**
		 * @brief  Compare colors
		 * @param  c1: Color 1
		 * @param  c2: Color 2
		 * @retval true if two colores are equal, false otherwise
		 */
		static bool ColorCompare(Color_t c1, Color_t c2);

		/**
		 * @brief  Generate a random color
		 * @note   Based on esp_random() function
		 * @retval Random color
		 */
		static Color_t GenerateRandomColor(void);

	protected:
		// NOTE: this display driver dose not support fast drawing, only defined for accommodation with GFX library
		void DrawFastVLine(int16_t x, int16_t y, int16_t h, Color_t color){};
		void DrawFastHLine(int16_t x, int16_t y, int16_t w, Color_t color){};

		/* ------------------------------- Subclasses ------------------------------- */
		/**
		 * @brief  A subclass that holds some predefined colors for display
		 * @note
		 * @retval None
		 */
	public:
		/**
		 * @brief  sub class that hold some predefined colors
		 * @note
		 */
		class Colors
		{
		public:
			static const Color_t OFF;
			static const Color_t RED;
			static const Color_t GREEN;
			static const Color_t BLUE;
			static const Color_t PURPLE;
			static const Color_t YELLOW;
			static const Color_t CYAN;
			static const Color_t WHITE;
		};

		/* --------------------------------- Objects -------------------------------- */
	public:
		static const Colors colors;

	protected:
		int16_t _width, _height;
		const bool supportFastLine = false;

	private:
		TickType_t _waitToBeFree;
		const uint32_t *pixelsMap = NULL;
		led_strip_t _strip;
		SemaphoreHandle_t displaySemaphore = NULL;
	};

}
