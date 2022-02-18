#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_system.h>

#include "Libraries/Display/LedStripDisplay.hpp"
#include "Libraries/GFX/GFX.h"
#include "Libraries/GFX/GFX.Text.cpp"
#include "Libraries/GFX/GFX.Draw.cpp"
#include "Libraries/GFX/Fonts/TomThumb.h"

const char tag[] = "main";

static const uint32_t displayPixelsMap[] = {0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51, 64, 65, 66, 67, 80, 81, 82, 83, 96, 97, 98, 99, 112, 113, 114, 115, 7, 6, 5, 4, 23, 22, 21, 20, 39, 38, 37, 36, 55, 54, 53, 52, 71, 70, 69, 68, 87, 86, 85, 84, 103, 102, 101, 100, 119, 118, 117, 116, 8, 9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59, 72, 73, 74, 75, 88, 89, 90, 91, 104, 105, 106, 107, 120, 121, 122, 123, 15, 14, 13, 12, 31, 30, 29, 28, 47, 46, 45, 44, 63, 62, 61, 60, 79, 78, 77, 76, 95, 94, 93, 92, 111, 110, 109, 108, 127, 126, 125, 124, 252, 253, 254, 255, 236, 237, 238, 239, 220, 221, 222, 223, 204, 205, 206, 207, 188, 189, 190, 191, 172, 173, 174, 175, 156, 157, 158, 159, 140, 141, 142, 143, 251, 250, 249, 248, 235, 234, 233, 232, 219, 218, 217, 216, 203, 202, 201, 200, 187, 186, 185, 184, 171, 170, 169, 168, 155, 154, 153, 152, 139, 138, 137, 136, 244, 245, 246, 247, 228, 229, 230, 231, 212, 213, 214, 215, 196, 197, 198, 199, 180, 181, 182, 183, 164, 165, 166, 167, 148, 149, 150, 151, 132, 133, 134, 135, 243, 242, 241, 240, 227, 226, 225, 224, 211, 210, 209, 208, 195, 194, 193, 192, 179, 178, 177, 176, 163, 162, 161, 160, 147, 146, 145, 144, 131, 130, 129, 128};
#define DISPLAY_W 32 // Display's width
#define DISPLAY_H 8	 // Display's length
EE::GFX<EE::LedStripDisplay, EE::LedStripDisplay::Color_t> gfx = EE::GFX<EE::LedStripDisplay, EE::LedStripDisplay::Color_t>(DISPLAY_W, DISPLAY_H);

void UpdateDisplay_task(void *pvParameters)
{
	for (;;)
	{
		if (gfx.Update() != ESP_OK)
		{
			ESP_LOGW(tag, "Failed to update display!");
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}

	vTaskDelete(NULL);
}

void WriteText_task(void *pvParameters)
{
	int16_t x = 1;
	int16_t y = 6;
	gfx.text.SetFont(&TomThumb);
	gfx.text.SetSize(1);
	const char *str = "ESP32";
	bool state = true;

	for (;;)
	{
		if (state)
		{
			gfx.text.SetTextColor(gfx.colors.PURPLE);
			gfx.text.SetCursor(1, 6);
			gfx.text.Write_Sync("ESP32");
		}
		else
		{
			gfx.text.SetTextColor(gfx.colors.CYAN);
			gfx.text.SetCursor(1, 6);
			gfx.text.Write_Sync("ESP32");
		}
		state = !state;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void DrawCircle_task(void *pvParameters)
{
	uint16_t ccX = 28;
	uint16_t ccY = 3;
	uint16_t maxR = 2;
	const uint16_t delay = 100;
	for (;;)
	{
		gfx.SetPixel(ccX, ccY, gfx.GenerateRandomColor());
		vTaskDelay(pdMS_TO_TICKS(delay));
		for (int16_t r = 1; r <= maxR; r++)
		{
			gfx.draw.Circle_Sync(ccX, ccY, r, gfx.GenerateRandomColor());
			vTaskDelay(pdMS_TO_TICKS(delay));
		}

		for (int16_t r = maxR; r >= 1; r--)
		{
			gfx.draw.FillCircle_Sync(ccX, ccY, r, gfx.GenerateRandomColor());
			vTaskDelay(pdMS_TO_TICKS(delay));
		}
		gfx.SetPixel(ccX, ccY, gfx.GenerateRandomColor());
		vTaskDelay(pdMS_TO_TICKS(delay));
	}

	gfx.draw.Line_Sync(0, 7, 31, 7, gfx.colors.YELLOW);
	while (1)
	{
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void DrawTriangle_task(void *pvParameters)
{
	int i = 0;
	uint16_t xOffset = 20;
	uint16_t yOffset = 1;
	for (;;)
	{
		switch (i)
		{
		case 0:
			gfx.draw.FillRect_Sync(xOffset + 0, yOffset + 0,
								   5, 5,
								   gfx.colors.GREEN);
			break;
		case 1:
			gfx.draw.FillTriangle_Sync(xOffset + 4, yOffset + 4,
									   xOffset + 4, yOffset + 0,
									   xOffset + 0, yOffset + 4,
									   gfx.colors.BLUE);
			break;
		case 2:
			gfx.draw.FillTriangle_Sync(xOffset + 0, yOffset + 0,
									   xOffset + 4, yOffset + 0,
									   xOffset + 0, yOffset + 4,
									   gfx.colors.RED);
			break;
		}

		i++;
		if (i == 3)
			i = 0;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void DrawFrame_task(void *pvParameters)
{
	static uint16_t position = 0;
	int16_t x = 0;
	int16_t y = 0;
	bool state = true; // true: turn on pixels, false: turn off pixels
	for (;;)
	{
		// Clear last point
		// gfx.SetPixel(x, y, gfx.colors.OFF);
		// position (linear) to x and y coordinates;
		if (position >= (DISPLAY_W + DISPLAY_H + DISPLAY_W))
		{
			// forth edge
			x = 0;
			y = (DISPLAY_H - 1) - (position - (DISPLAY_W + DISPLAY_H + DISPLAY_W));
		}
		else if (position >= (DISPLAY_W + DISPLAY_H))
		{
			// third edge
			x = (DISPLAY_W - 1) - (position - (DISPLAY_W + DISPLAY_H));
			y = DISPLAY_H - 1;
		}
		else if (position >= DISPLAY_W)
		{
			// second edge
			x = DISPLAY_W - 1;
			y = position - DISPLAY_W;
		}
		else
		{
			// fist edge
			x = position;
			y = 0;
		}
		if (state)
			gfx.SetPixel(x, y, gfx.GenerateRandomColor());
		else
			gfx.SetPixel(x, y, gfx.colors.OFF);

		position++;
		if (position == (2 * DISPLAY_H + 2 * DISPLAY_W))
		{
			position = 0;
			state = !state;
		}

		vTaskDelay(pdMS_TO_TICKS(20));
	}
}
extern "C"
{
	void app_main(void);
}

void app_main(void)
{
	gfx.Init(LED_STRIP_WS2812, GPIO_NUM_14, RMT_CHANNEL_0, 10, displayPixelsMap);
	xTaskCreate(UpdateDisplay_task, "UpdateDisplay_task", 1024 * 2, NULL, 5, NULL);

	xTaskCreate(DrawCircle_task, "DrawCircle_task", 1024 * 2, NULL, 5, NULL);
	xTaskCreate(DrawTriangle_task, "DrawTriangle_task", 1024 * 2, NULL, 5, NULL);
	xTaskCreate(DrawFrame_task, "DrawFrame_task", 1024 * 2, NULL, 5, NULL);
	xTaskCreate(WriteText_task, "WriteText_task", 1024 * 2, NULL, 5, NULL);
}
