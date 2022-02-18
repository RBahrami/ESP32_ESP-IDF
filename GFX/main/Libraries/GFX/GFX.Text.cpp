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
#include "glcdfont.c"

namespace EE
{
/* -------------------------------------------------------------------------- */
/*                     Private Macros and inline fuctions                     */
/* -------------------------------------------------------------------------- */
#define ReadUint8(addr) (*(const unsigned char *)(addr))
#define ReadUint16(addr) (*(const unsigned short *)(addr))

	static inline GfxGlyph_t *ReadGlyphPointer(const GfxFont_t *gfxFont, uint8_t c)
	{
		return gfxFont->glyph + c;
	}

	static inline uint8_t *ReadBitmapPointer(const GfxFont_t *gfxFont)
	{
		return gfxFont->bitmap;
	}

	/* -------------------------------------------------------------------------- */
	/*               Text related member functions (Sync and Async)               */
	/* -------------------------------------------------------------------------- */
	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::DrawChar_ASync(int16_t x, int16_t y, unsigned char c, Color_t color, Color_t bg, uint8_t size_x, uint8_t size_y)
	{

		if (!gfxFont)
		{ // 'Classic' built-in font

			if ((x >= parent._width) ||		  // Clip right
				(y >= parent._height) ||	  // Clip bottom
				((x + 6 * size_x - 1) < 0) || // Clip left
				((y + 8 * size_y - 1) < 0))	  // Clip top
				return;

			if (!_cp437 && (c >= 176))
				c++; // Handle 'classic' charset behavior

			for (int8_t i = 0; i < 5; i++)
			{ // Char bitmap = 5 columns
				uint8_t line = ReadUint8(&font[c * 5 + i]);
				for (int8_t j = 0; j < 8; j++, line >>= 1)
				{
					if (line & 1)
					{
						if (size_x == 1 && size_y == 1)
							parent.SetPixel(x + i, y + j, color);
						else
							parent.draw.FillRect_Async(x + i * size_x, y + j * size_y, size_x, size_y, color);
					}
					else if (!parent.ColorCompare(bg, color))
					{
						if (size_x == 1 && size_y == 1)
							parent.SetPixel(x + i, y + j, bg);
						else
							parent.draw.FillRect_Async(x + i * size_x, y + j * size_y, size_x, size_y, bg);
					}
				}
			}
			if (!parent.ColorCompare(bg, color))
			{ // If opaque, draw vertical line for last column
				if (size_x == 1 && size_y == 1)
					parent.draw.VLine_Async(x + 5, y, 8, bg);
				else
					parent.draw.FillRect_Async(x + 5 * size_x, y, size_x, 8 * size_y, bg);
			}
		}
		else
		{ // Custom font

			// Character is assumed previously filtered by write() to eliminate
			// newlines, returns, non-printable characters, etc.  Calling
			// drawChar() directly with 'bad' characters of font may cause mayhem!

			c -= (uint8_t)ReadUint8(&gfxFont->first);
			GfxGlyph_t *glyph = ReadGlyphPointer(gfxFont, c);
			uint8_t *bitmap = ReadBitmapPointer(gfxFont);

			uint16_t bo = ReadUint16(&glyph->bitmapOffset);
			uint8_t w = ReadUint8(&glyph->width), h = ReadUint8(&glyph->height);
			int8_t xo = ReadUint8(&glyph->xOffset),
				   yo = ReadUint8(&glyph->yOffset);
			uint8_t xx, yy, bits = 0, bit = 0;
			int16_t xo16 = 0, yo16 = 0;

			if (size_x > 1 || size_y > 1)
			{
				xo16 = xo;
				yo16 = yo;
			}

			// Todo: Add character clipping here

			// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
			// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
			// has typically been used with the 'classic' font to overwrite old
			// screen contents with new data.  This ONLY works because the
			// characters are a uniform size; it's not a sensible thing to do with
			// proportionally-spaced fonts with glyphs of varying sizes (and that
			// may overlap).  To replace previously-drawn text when using a custom
			// font, use the GetTextBounds() function to determine the smallest
			// rectangle encompassing a string, erase the area with fillRect(),
			// then draw new text.  This WILL infortunately 'blink' the text, but
			// is unavoidable.  Drawing 'background' pixels will NOT fix this,
			// only creates a new set of problems.  Have an idea to work around
			// this (a canvas object type for MCUs that can afford the RAM and
			// displays supporting setAddrWindow() and pushColors()), but haven't
			// implemented this yet.

			for (yy = 0; yy < h; yy++)
			{
				for (xx = 0; xx < w; xx++)
				{
					if (!(bit++ & 7))
					{
						bits = ReadUint8(&bitmap[bo++]);
					}
					if (bits & 0x80)
					{
						if (size_x == 1 && size_y == 1)
						{
							parent.SetPixel(x + xo + xx, y + yo + yy, color);
						}
						else
						{
							parent.draw.FillRect_Async(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y, size_x, size_y, color);
						}
					}
					bits <<= 1;
				}
			}
		} // End classic vs custom font
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::DrawChar_Sync(int16_t x, int16_t y, unsigned char c, Color_t color, Color_t bg, uint8_t size_x, uint8_t size_y)
	{
		parent.StartWrite();
		DrawChar_ASync(x, y, c, color, bg, size_x, size_y);
		parent.StopWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::Write_Async(uint8_t c)
	{
		if (!gfxFont)
		{ // 'Classic' built-in font

			if (c == '\n')
			{								// Newline?
				cursor_x = 0;				// Reset x to zero,
				cursor_y += textsize_y * 8; // advance y one line
			}
			else if (c != '\r')
			{ // Ignore carriage returns
				if (wrap && ((cursor_x + textsize_x * 6) > parent._width))
				{								// Off right?
					cursor_x = 0;				// Reset x to zero,
					cursor_y += textsize_y * 8; // advance y one line
				}
				DrawChar_ASync(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
				cursor_x += textsize_x * 6; // Advance x one char
			}
		}
		else
		{ // Custom font
			if (c == '\n')
			{
				cursor_x = 0;
				cursor_y += (int16_t)textsize_y * (uint8_t)ReadUint8(&gfxFont->yAdvance);
			}
			else if (c != '\r')
			{
				uint8_t first = ReadUint8(&gfxFont->first);
				if ((c >= first) && (c <= (uint8_t)ReadUint8(&gfxFont->last)))
				{
					GfxGlyph_t *glyph = ReadGlyphPointer(gfxFont, c - first);
					uint8_t w = ReadUint8(&glyph->width),
							h = ReadUint8(&glyph->height);
					if ((w > 0) && (h > 0))
					{													 // Is there an associated bitmap?
						int16_t xo = (int8_t)ReadUint8(&glyph->xOffset); // sic
						if (wrap && ((cursor_x + textsize_x * (xo + w)) > parent._width))
						{
							cursor_x = 0;
							cursor_y += (int16_t)textsize_y *
										(uint8_t)ReadUint8(&gfxFont->yAdvance);
						}
						DrawChar_ASync(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
					}
					cursor_x += (uint8_t)ReadUint8(&glyph->xAdvance) * (int16_t)textsize_x;
				}
			}
		}
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::Write_Sync(char *str)
	{
		parent.StartWrite();
		while (*str)
			Write_Async(*str++);
		parent.EndWrite();
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::SetSize(uint8_t s)
	{
		SetSize(s, s);
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::SetSize(uint8_t s_x, uint8_t s_y)
	{
		textsize_x = (s_x > 0) ? s_x : 1;
		textsize_y = (s_y > 0) ? s_y : 1;
	}

	/**
		@brief Set the font to display when print()ing, either custom or default
		@param  f  The GfxFont_t object, if NULL use built in 6x8 font
	*/
	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::SetFont(const GfxFont_t *f)
	{
		if (f)
		{ // Font struct pointer passed in?
			if (!gfxFont)
			{ // And no current font struct?
				// Switching from classic to new font behavior.
				// Move cursor pos down 6 pixels so it's on baseline.
				cursor_y += 6;
			}
		}
		else if (gfxFont)
		{ // NULL passed.  Current font struct defined?
			// Switching from new to classic font behavior.
			// Move cursor pos up 6 pixels so it's at top-left of char.
			cursor_y -= 6;
		}
		gfxFont = (GfxFont_t *)f;
	}

	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::GetCharBounds(unsigned char c, int16_t *x, int16_t *y,
													  int16_t *minx, int16_t *miny, int16_t *maxx,
													  int16_t *maxy)
	{

		if (gfxFont)
		{

			if (c == '\n')
			{			// Newline?
				*x = 0; // Reset x to zero, advance y by one line
				*y += textsize_y * (uint8_t)ReadUint8(&gfxFont->yAdvance);
			}
			else if (c != '\r')
			{ // Not a carriage return; is normal char
				uint8_t first = ReadUint8(&gfxFont->first),
						last = ReadUint8(&gfxFont->last);
				if ((c >= first) && (c <= last))
				{ // Char present in this font?
					GfxGlyph_t *glyph = ReadGlyphPointer(gfxFont, c - first);
					uint8_t gw = ReadUint8(&glyph->width),
							gh = ReadUint8(&glyph->height),
							xa = ReadUint8(&glyph->xAdvance);
					int8_t xo = ReadUint8(&glyph->xOffset),
						   yo = ReadUint8(&glyph->yOffset);
					if (wrap && ((*x + (((int16_t)xo + gw) * textsize_x)) > parent._width))
					{
						*x = 0; // Reset x to zero, advance y by one line
						*y += textsize_y * (uint8_t)ReadUint8(&gfxFont->yAdvance);
					}
					int16_t tsx = (int16_t)textsize_x, tsy = (int16_t)textsize_y,
							x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + gw * tsx - 1,
							y2 = y1 + gh * tsy - 1;
					if (x1 < *minx)
						*minx = x1;
					if (y1 < *miny)
						*miny = y1;
					if (x2 > *maxx)
						*maxx = x2;
					if (y2 > *maxy)
						*maxy = y2;
					*x += xa * tsx;
				}
			}
		}
		else
		{ // Default font

			if (c == '\n')
			{						  // Newline?
				*x = 0;				  // Reset x to zero,
				*y += textsize_y * 8; // advance y one line
									  // min/max x/y unchaged -- that waits for next 'normal' character
			}
			else if (c != '\r')
			{ // Normal char; ignore carriage returns
				if (wrap && ((*x + textsize_x * 6) > parent._width))
				{						  // Off right?
					*x = 0;				  // Reset x to zero,
					*y += textsize_y * 8; // advance y one line
				}
				int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
					y2 = *y + textsize_y * 8 - 1;
				if (x2 > *maxx)
					*maxx = x2; // Track max x, y
				if (y2 > *maxy)
					*maxy = y2;
				if (*x < *minx)
					*minx = *x; // Track min x, y
				if (*y < *miny)
					*miny = *y;
				*x += textsize_x * 6; // Advance x one char
			}
		}
	}

	/**
		@brief  Helper to determine size of a string with current font/size.
				Pass string and a cursor position, returns UL corner and W,H.
		@param  str  The ASCII string to measure
		@param  x    The current cursor X
		@param  y    The current cursor Y
		@param  x1   The boundary X coordinate, returned by function
		@param  y1   The boundary Y coordinate, returned by function
		@param  w    The boundary width, returned by function
		@param  h    The boundary height, returned by function
	*/
	template <class Display_t, typename Color_t>
	void GFX<Display_t, Color_t>::Text::GetTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
	{
		uint8_t c;													// Current character
		int16_t minx = 0x7FFF, miny = 0x7FFF, maxx = -1, maxy = -1; // Bound rect
		// Bound rect is intentionally initialized inverted, so 1st char sets it

		*x1 = x; // Initial position is value passed in
		*y1 = y;
		*w = *h = 0; // Initial size is zero

		while ((c = *str++))
		{
			// GetCharBounds() modifies x/y to advance for each character,
			// and min/max x/y are updated to incrementally build bounding rect.
			GetCharBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
		}

		if (maxx >= minx)
		{						  // If legit string bounds were found...
			*x1 = minx;			  // Update x1 to least X coord,
			*w = maxx - minx + 1; // And w to bound rect width
		}
		if (maxy >= miny)
		{ // Same for height
			*y1 = miny;
			*h = maxy - miny + 1;
		}
	}
}
