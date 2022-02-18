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

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_system.h>

#include "Libraries/Display/LedStripDisplay.hpp"

const char tag[] = "main";

// array to hold pixel map of display
// driver will use this array to map virtual pixel number to physical pixel number in display
// modify it based on your hardware
static const uint32_t displayPixelsMap[] = {0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51, 64, 65, 66, 67, 80, 81, 82, 83, 96, 97, 98, 99, 112, 113, 114, 115, 7, 6, 5, 4, 23, 22, 21, 20, 39, 38, 37, 36, 55, 54, 53, 52, 71, 70, 69, 68, 87, 86, 85, 84, 103, 102, 101, 100, 119, 118, 117, 116, 8, 9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59, 72, 73, 74, 75, 88, 89, 90, 91, 104, 105, 106, 107, 120, 121, 122, 123, 15, 14, 13, 12, 31, 30, 29, 28, 47, 46, 45, 44, 63, 62, 61, 60, 79, 78, 77, 76, 95, 94, 93, 92, 111, 110, 109, 108, 127, 126, 125, 124, 252, 253, 254, 255, 236, 237, 238, 239, 220, 221, 222, 223, 204, 205, 206, 207, 188, 189, 190, 191, 172, 173, 174, 175, 156, 157, 158, 159, 140, 141, 142, 143, 251, 250, 249, 248, 235, 234, 233, 232, 219, 218, 217, 216, 203, 202, 201, 200, 187, 186, 185, 184, 171, 170, 169, 168, 155, 154, 153, 152, 139, 138, 137, 136, 244, 245, 246, 247, 228, 229, 230, 231, 212, 213, 214, 215, 196, 197, 198, 199, 180, 181, 182, 183, 164, 165, 166, 167, 148, 149, 150, 151, 132, 133, 134, 135, 243, 242, 241, 240, 227, 226, 225, 224, 211, 210, 209, 208, 195, 194, 193, 192, 179, 178, 177, 176, 163, 162, 161, 160, 147, 146, 145, 144, 131, 130, 129, 128};

#define DISPLAY_W 32 // Display's width
#define DISPLAY_H 8	 // Display's length

EE::LedStripDisplay display = EE::LedStripDisplay(DISPLAY_W, DISPLAY_H);

extern "C"
{
	void app_main(void);
}

void app_main(void)
{
	// Set type of led strip pixels, gpio number that strip is connected to, RMT channel, default brightness and pointer to pixles map array
	display.Init(LED_STRIP_WS2812, GPIO_NUM_14, RMT_CHANNEL_0, 10, displayPixelsMap);

	// Test code
	for (;;)
	{
		// Fill display with random colors
		for (int16_t x = 0; x < DISPLAY_W; x++)
		{
			for (int16_t y = 0; y < DISPLAY_H; y++)
			{
				display.SetPixel(x, y, display.GenerateRandomColor());
				display.Update();
				vTaskDelay(pdMS_TO_TICKS(40));
			}
		}
	}
}
