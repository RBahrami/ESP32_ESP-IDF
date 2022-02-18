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

#include "LedStripDisplay.hpp"
static const char tag[] = "display";

namespace EE
{
	typedef LedStripDisplay LSD; // To shorten the name of the class and make life easier :)

	void LSD::Init(led_strip_type_t type, gpio_num_t gpioNumber, rmt_channel_t rmtChannel, float brightness, const uint32_t *pixelsMap_p)
	{
		pixelsMap = pixelsMap_p;
		uint8_t _brightness = (uint8_t)((brightness / 100.0) * 255.0);
		_strip = {
			.type = type,
			.is_rgbw = false,
			.brightness = _brightness,
			.length = (size_t)(_width * _height),
			.gpio = gpioNumber,
			.channel = rmtChannel, // TODO: make configurable
			.buf = NULL,
		};

		ESP_LOGI(tag, "Initializeing LED strip display...");
		led_strip_install();
		esp_err_t err = led_strip_init(&_strip);
		if (err == ESP_OK)
			ESP_LOGI(tag, "OK");
		else
			ESP_LOGE(tag, "Failed, Error: %s", esp_err_to_name(err));
	}

	void LSD::SetPixel(int16_t x, int16_t y, LSD::Color_t color)
	{
		// TODO: Check if x and y is in display range
		size_t pixelNumber = x + (y * _width);
		if (pixelsMap) // If a pixels map array is provided at initialization, convert virtual pixel number to physical one
			pixelNumber = pixelsMap[pixelNumber];
		led_strip_set_pixel(&_strip, pixelNumber, color);
	}

	esp_err_t LSD::Update(void)
	{
		esp_err_t err = ESP_ERR_TIMEOUT;
		if (StartWrite(_waitToBeFree) == ESP_OK)
		{
			err = led_strip_flush(&_strip);
			EndWrite();
		}
		return err;
	}

	esp_err_t LSD::StartWrite(const TickType_t ticks)
	{
		if (xSemaphoreTake(displaySemaphore, ticks) == pdTRUE)
		{
			return ESP_OK;
		}
		return ESP_ERR_TIMEOUT;
	}

	void LSD::EndWrite()
	{
		xSemaphoreGive(displaySemaphore);
	}

	void LSD::SetBrightness(float brightness)
	{
		uint8_t _brightness = (uint8_t)((brightness / 100.0) * 255.0);
		_strip.brightness = _brightness;
	}

	LSD::Color_t LSD::GenerateRandomColor(void)
	{
		LSD::Color_t randomColor;
		esp_fill_random((void *)&randomColor, sizeof(LSD::Color_t));
		return randomColor;
	}

	bool LSD::ColorCompare(LSD::Color_t c1, LSD::Color_t c2)
	{
		if (c1.r != c2.r)
			return false;
		if (c1.g != c2.g)
			return false;
		if (c1.b != c2.b)
			return false;
		return true;
	}

	const LSD::Color_t LSD::Colors::OFF = {.r = 0x00, .g = 0x00, .b = 0x00};
	const LSD::Color_t LSD::Colors::RED = {.r = 0xFF, .g = 0x00, .b = 0x00};
	const LSD::Color_t LSD::Colors::GREEN = {.r = 0x00, .g = 0xFF, .b = 0x00};
	const LSD::Color_t LSD::Colors::BLUE = {.r = 0x00, .g = 0x00, .b = 0xFF};
	const LSD::Color_t LSD::Colors::PURPLE = {.r = 0xFF, .g = 0x00, .b = 0xFF};
	const LSD::Color_t LSD::Colors::YELLOW = {.r = 0xFF, .g = 0xFF, .b = 0x00};
	const LSD::Color_t LSD::Colors::CYAN = {.r = 0xFF, .g = 0xFF, .b = 0x00};
	const LSD::Color_t LSD::Colors::WHITE = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
}
