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
// This library is based on adafruit GFX library for arduino with BSD license
/*
Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "GFX.h"

namespace EE
{
#define SwapInt16(a, b) \
	{                   \
		int16_t t = a;  \
		a = b;          \
		b = t;          \
	}

	/* -------------------------------------------------------------------------- */
	/*                       Asynchronies drawing functions                       */
	/* -------------------------------------------------------------------------- */
	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::_Line_Async(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color_t color)
	{
		int16_t steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep)
		{
			SwapInt16(x0, y0);
			SwapInt16(x1, y1);
		}

		if (x0 > x1)
		{
			SwapInt16(x0, x1);
			SwapInt16(y0, y1);
		}

		int16_t dx, dy;
		dx = x1 - x0;
		dy = abs(y1 - y0);

		int16_t err = dx / 2;
		int16_t ystep;

		if (y0 < y1)
		{
			ystep = 1;
		}
		else
		{
			ystep = -1;
		}

		for (; x0 <= x1; x0++)
		{
			if (steep)
			{
				parent.SetPixel(y0, x0, color);
			}
			else
			{
				parent.SetPixel(x0, y0, color);
			}
			err -= dy;
			if (err < 0)
			{
				y0 += ystep;
				err += dx;
			}
		}
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::VLine_Async(int16_t x, int16_t y, int16_t h, Color_t color)
	{
		if (parent.supportFastLine)
			parent.DrawFastVLine(x, y, h, color);
		else
			_Line_Async(x, y, x, y + h - 1, color);
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::HLine_Async(int16_t x, int16_t y, int16_t w, Color_t color)
	{
		if (parent.supportFastLine)
			parent.DrawFastHLine(x, y, w, color);
		else
			_Line_Async(x, y, x + w - 1, y, color);
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::Line_Async(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color_t color)
	{
		if (x0 == x1)
		{
			if (y0 > y1)
				SwapInt16(y0, y1);
			VLine_Async(x0, y0, y1 - y0 + 1, color);
		}
		else if (y0 == y1)
		{
			if (x0 > x1)
				SwapInt16(x0, x1);
			HLine_Async(x0, y0, x1 - x0 + 1, color);
		}
		else
		{
			parent.StartWrite();
			Line_Async(x0, y0, x1, y1, color);
			parent.EndWrite();
		}
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillRect_Async(int16_t x, int16_t y, int16_t w, int16_t h, Color_t color)
	{
		for (int16_t i = x; i < x + w; i++)
			VLine_Async(i, y, h, color);
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::CircleHelper_Async(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, Color_t color)
	{
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;

		while (x < y)
		{
			if (f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
			if (cornername & 0x4)
			{
				parent.SetPixel(x0 + x, y0 + y, color);
				parent.SetPixel(x0 + y, y0 + x, color);
			}
			if (cornername & 0x2)
			{
				parent.SetPixel(x0 + x, y0 - y, color);
				parent.SetPixel(x0 + y, y0 - x, color);
			}
			if (cornername & 0x8)
			{
				parent.SetPixel(x0 - y, y0 + x, color);
				parent.SetPixel(x0 - x, y0 + y, color);
			}
			if (cornername & 0x1)
			{
				parent.SetPixel(x0 - y, y0 - x, color);
				parent.SetPixel(x0 - x, y0 - y, color);
			}
		}
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillCircleHelper_Async(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, Color_t color)
	{
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;
		int16_t px = x;
		int16_t py = y;

		delta++; // Avoid some +1's in the loop

		while (x < y)
		{
			if (f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
			// These checks avoid double-drawing certain lines, important
			// for the SSD1306 library which has an INVERT drawing mode.
			if (x < (y + 1))
			{
				if (corners & 1)
					VLine_Async(x0 + x, y0 - y, 2 * y + delta, color);
				if (corners & 2)
					VLine_Async(x0 - x, y0 - y, 2 * y + delta, color);
			}
			if (y != py)
			{
				if (corners & 1)
					VLine_Async(x0 + py, y0 - px, 2 * px + delta, color);
				if (corners & 2)
					VLine_Async(x0 - py, y0 - px, 2 * px + delta, color);
				py = y;
			}
			px = x;
		}
	}

	/* -------------------------------------------------------------------------- */
	/*                        Synchronized Drawing fuctions                       */
	/* -------------------------------------------------------------------------- */
	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::Line_Sync(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color_t color)
	{
		parent.StartWrite();
		Line_Async(x0, y0, x1, y1, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::Rect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, Color_t color)
	{
		parent.StartWrite();
		HLine_Async(x, y, w, color);
		HLine_Async(x, y + h - 1, w, color);
		VLine_Async(x, y, h, color);
		VLine_Async(x + w - 1, y, h, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillRect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, Color_t color)
	{
		parent.StartWrite();
		FillRect_Async(x, y, w, h, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::RoundRect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, Color_t color)
	{
		int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
		if (r > max_radius)
			r = max_radius;
		// smarter version
		parent.StartWrite();
		HLine_Async(x + r, y, w - 2 * r, color);		 // Top
		HLine_Async(x + r, y + h - 1, w - 2 * r, color); // Bottom
		VLine_Async(x, y + r, h - 2 * r, color);		 // Left
		VLine_Async(x + w - 1, y + r, h - 2 * r, color); // Right
		// draw four corners
		DrawCircleHelper_Async(x + r, y + r, r, 1, color);
		DrawCircleHelper_Async(x + w - r - 1, y + r, r, 2, color);
		DrawCircleHelper_Async(x + w - r - 1, y + h - r - 1, r, 4, color);
		DrawCircleHelper_Async(x + r, y + h - r - 1, r, 8, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillRoundRect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, Color_t color)
	{
		int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
		if (r > max_radius)
			r = max_radius;
		// smarter version
		parent.StartWrite();
		writeFillRect(x + r, y, w - 2 * r, h, color);
		// draw four corners
		fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
		fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillScreen_Sync(Color_t color)
	{
		DrawFillRect_Sync(0, 0, parent._width, parent._height, color);
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::Circle_Sync(int16_t x0, int16_t y0, int16_t r, Color_t color)
	{
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;

		parent.StartWrite();
		parent.SetPixel(x0, y0 + r, color);
		parent.SetPixel(x0, y0 - r, color);
		parent.SetPixel(x0 + r, y0, color);
		parent.SetPixel(x0 - r, y0, color);

		while (x < y)
		{
			if (f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;

			parent.SetPixel(x0 + x, y0 + y, color);
			parent.SetPixel(x0 - x, y0 + y, color);
			parent.SetPixel(x0 + x, y0 - y, color);
			parent.SetPixel(x0 - x, y0 - y, color);
			parent.SetPixel(x0 + y, y0 + x, color);
			parent.SetPixel(x0 - y, y0 + x, color);
			parent.SetPixel(x0 + y, y0 - x, color);
			parent.SetPixel(x0 - y, y0 - x, color);
		}
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillCircle_Sync(int16_t x0, int16_t y0, int16_t r, Color_t color)
	{
		parent.StartWrite();
		VLine_Async(x0, y0 - r, 2 * r + 1, color);
		FillCircleHelper_Async(x0, y0, r, 3, 0, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::Triangle_Sync(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color_t color)
	{
		parent.StartWrite();
		Line_Async(x0, y0, x1, y1, color);
		Line_Async(x1, y1, x2, y2, color);
		Line_Async(x2, y2, x0, y0, color);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Draw::FillTriangle_Sync(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color_t color)
	{

		int16_t a, b, y, last;

		// Sort coordinates by Y order (y2 >= y1 >= y0)
		if (y0 > y1)
		{
			SwapInt16(y0, y1);
			SwapInt16(x0, x1);
		}
		if (y1 > y2)
		{
			SwapInt16(y2, y1);
			SwapInt16(x2, x1);
		}
		if (y0 > y1)
		{
			SwapInt16(y0, y1);
			SwapInt16(x0, x1);
		}

		parent.StartWrite();
		if (y0 == y2)
		{ // Handle awkward all-on-same-line case as its own thing
			a = b = x0;
			if (x1 < a)
				a = x1;
			else if (x1 > b)
				b = x1;
			if (x2 < a)
				a = x2;
			else if (x2 > b)
				b = x2;
			HLine_Async(a, y0, b - a + 1, color);
			parent.EndWrite();
			return;
		}

		int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
				dx12 = x2 - x1, dy12 = y2 - y1;
		int32_t sa = 0, sb = 0;

		// For upper part of triangle, find scanline crossings for segments
		// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
		// is included here (and second loop will be skipped, avoiding a /0
		// error there), otherwise scanline y1 is skipped here and handled
		// in the second loop...which also avoids a /0 error here if y0=y1
		// (flat-topped triangle).
		if (y1 == y2)
			last = y1; // Include y1 scanline
		else
			last = y1 - 1; // Skip it

		for (y = y0; y <= last; y++)
		{
			a = x0 + sa / dy01;
			b = x0 + sb / dy02;
			sa += dx01;
			sb += dx02;
			/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
			if (a > b)
				SwapInt16(a, b);
			HLine_Async(a, y, b - a + 1, color);
		}

		// For lower part of triangle, find scanline crossings for segments
		// 0-2 and 1-2.  This loop is skipped if y1=y2.
		sa = (int32_t)dx12 * (y - y1);
		sb = (int32_t)dx02 * (y - y0);
		for (; y <= y2; y++)
		{
			a = x1 + sa / dy12;
			b = x0 + sb / dy02;
			sa += dx12;
			sb += dx02;
			/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
			if (a > b)
				SwapInt16(a, b);
			HLine_Async(a, y, b - a + 1, color);
		}
		parent.EndWrite();
	}
}
