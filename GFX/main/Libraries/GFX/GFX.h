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
// The Draw and Test part of this library is based on adafruit GFX library for arduino
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
/*
* This library provides some basic graphic function for displays. 
* For more information refer to README.md in example project.
*/
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gfxfont.h"
namespace EE
{
	template <class Display_t, typename Color_t>
	class GFX : public Display_t
	{
		class Draw
		{
			GFX &parent;

		public:
			// Constructor
			Draw(GFX &_parent) : parent(_parent) {}

			/* -------------------------------------------------------------------------- */
			/*                       Asynchronies drawing functions                       */
			/* -------------------------------------------------------------------------- */
			/**
				@brief    Draw line - Async.  Bresenham's algorithm - thx wikipedia
				@param    x0  Start point x coordinate
				@param    y0  Start point y coordinate
				@param    x1  End point x coordinate
				@param    y1  End point y coordinate
				@param    color Color to draw with
			*/
		private:
			void _Line_Async(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color_t color);

		public:
			/**
				@brief    Draw a perfectly vertical line - Async
				@param    x   Top-most x coordinate
				@param    y   Top-most y coordinate
				@param    h   Height in pixels
				@param    color Color to fill with
			*/
			void VLine_Async(int16_t x, int16_t y, int16_t h, Color_t color);

			/**
			   @brief    Draw a perfectly horizontal line - Async
				@param    x   Left-most x coordinate
				@param    y   Left-most y coordinate
				@param    w   Width in pixels
			   @param    color Color to fill with
			*/
			void HLine_Async(int16_t x, int16_t y, int16_t w, Color_t color);

			/**
			   @brief    Helper function to draw a line - Async
				@param    x0  Start point x coordinate
				@param    y0  Start point y coordinate
				@param    x1  End point x coordinate
				@param    y1  End point y coordinate
				@param    color Color to draw with
			*/
			void Line_Async(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color_t color);

			/**
				@brief    Fill a rectangle completely with one color - Async
				@param    x   Top left corner x coordinate
				@param    y   Top left corner y coordinate
				@param    w   Width in pixels
				@param    h   Height in pixels
				@param    color Color to fill with
			*/
			void FillRect_Async(int16_t x, int16_t y, int16_t w, int16_t h, Color_t color);

			/**
				@brief    Quarter-circle drawer, used to do circles and roundrects - Async
				@param    x0   Center-point x coordinate
				@param    y0   Center-point y coordinate
				@param    r   Radius of circle
				@param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
				@param    color Color to draw with
			*/
			void CircleHelper_Async(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, Color_t color);

			/**
				@brief  Quarter-circle drawer with fill, used for circles and roundrects
				@param  x0       Center-point x coordinate
				@param  y0       Center-point y coordinate
				@param  r        Radius of circle
				@param  corners  Mask bits indicating which quarters we're doing
				@param  delta    Offset from center-point, used for round-rects
				@param  color    Color to fill with
			*/
			void FillCircleHelper_Async(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, Color_t color);

			/* -------------------------------------------------------------------------- */
			/*                        Synchronized Drawing fuctions                       */
			/* -------------------------------------------------------------------------- */
			/**
			   @brief    Helper function to draw a line - Sync
				@param    x0  Start point x coordinate
				@param    y0  Start point y coordinate
				@param    x1  End point x coordinate
				@param    y1  End point y coordinate
				@param    color Color to draw with
			*/
			void Line_Sync(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color_t color);

			/**
			   @brief   Draw a rectangle with no fill color - Sync
				@param    x   Top left corner x coordinate
				@param    y   Top left corner y coordinate
				@param    w   Width in pixels
				@param    h   Height in pixels
				@param    color Color to draw with
			*/
			void Rect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, Color_t color);

			/**
				@brief    Fill a rectangle completely with one color - Sync.
				@param    x   Top left corner x coordinate
				@param    y   Top left corner y coordinate
				@param    w   Width in pixels
				@param    h   Height in pixels
				@param    color Color to fill with
			*/
			void FillRect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, Color_t color);

			/**
			   @brief   Draw a rounded rectangle with no fill color - Sync
				@param    x   Top left corner x coordinate
				@param    y   Top left corner y coordinate
				@param    w   Width in pixels
				@param    h   Height in pixels
				@param    r   Radius of corner rounding
				@param    color Color to draw with
			*/
			void RoundRect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, Color_t color);

			/**
			   @brief   Draw a rounded rectangle with fill color - Sync
				@param    x   Top left corner x coordinate
				@param    y   Top left corner y coordinate
				@param    w   Width in pixels
				@param    h   Height in pixels
				@param    r   Radius of corner rounding
				@param    color Color to draw/fill with
			*/
			void FillRoundRect_Sync(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, Color_t color);

			/**
				@brief    Fill the screen completely with one color - Sync.
				@param    color Color to fill with
			*/
			void FillScreen_Sync(Color_t color);

			/**
			   @brief    Draw a circle outline - Sync
				@param    x0   Center-point x coordinate
				@param    y0   Center-point y coordinate
				@param    r   Radius of circle
				@param    color Color to draw with
			*/
			void Circle_Sync(int16_t x0, int16_t y0, int16_t r, Color_t color);

			/**
			   @brief    Draw a circle with filled color - Sync
				@param    x0   Center-point x coordinate
				@param    y0   Center-point y coordinate
				@param    r   Radius of circle
				@param    color Color to fill with
			*/
			void FillCircle_Sync(int16_t x0, int16_t y0, int16_t r, Color_t color);

			/**
			   @brief   Draw a triangle with no fill color - Sync
				@param    x0  Vertex #0 x coordinate
				@param    y0  Vertex #0 y coordinate
				@param    x1  Vertex #1 x coordinate
				@param    y1  Vertex #1 y coordinate
				@param    x2  Vertex #2 x coordinate
				@param    y2  Vertex #2 y coordinate
				@param    color Color to draw with
			*/
			void Triangle_Sync(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color_t color);

			/**
			   @brief     Draw a triangle with color-fill - Sync
				@param    x0  Vertex #0 x coordinate
				@param    y0  Vertex #0 y coordinate
				@param    x1  Vertex #1 x coordinate
				@param    y1  Vertex #1 y coordinate
				@param    x2  Vertex #2 x coordinate
				@param    y2  Vertex #2 y coordinate
				@param    color Color to fill/draw with
			*/
			void FillTriangle_Sync(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, Color_t color);
		};

		class Text
		{
			GFX &parent;

		public:
			// Constructor
			Text(GFX &_parent) : parent(_parent)
			{
				cursor_y = cursor_x = 0;
				textsize_x = textsize_y = 1;
				wrap = true;
				_cp437 = false;
				gfxFont = NULL;
			}

			/* -------------------------------------------------------------------------- */
			/*               Text related member functions (Sync and Async)               */
			/* -------------------------------------------------------------------------- */
			/**
			   @brief   Draw a single character - Async
				@param    x   Bottom left corner x coordinate
				@param    y   Bottom left corner y coordinate
				@param    c   The 8-bit font-indexed character (likely ascii)
				@param    color Color to draw chraracter with
				@param    bg Color to fill background with (if same as color,
			   no background)
				@param    size_x  Font magnification level in X-axis, 1 is 'original' size
				@param    size_y  Font magnification level in Y-axis, 1 is 'original' size
			*/
			void DrawChar_ASync(int16_t x, int16_t y, unsigned char c, Color_t color, Color_t bg, uint8_t size_x, uint8_t size_y);

			/**
			   @brief   Draw a single character - Sync
				@param    x   Bottom left corner x coordinate
				@param    y   Bottom left corner y coordinate
				@param    c   The 8-bit font-indexed character (likely ascii)
				@param    color Color to draw chraracter with
				@param    bg Color to fill background with (if same as color,
			   no background)
				@param    size_x  Font magnification level in X-axis, 1 is 'original' size
				@param    size_y  Font magnification level in Y-axis, 1 is 'original' size
			*/
			void DrawChar_Sync(int16_t x, int16_t y, unsigned char c, Color_t color, Color_t bg, uint8_t size_x, uint8_t size_y);

			/**
				@brief  Print one byte/character of data - Async
				@param  c  The 8-bit ascii character to write
			*/
			void Write_Async(uint8_t c);

			/**
			 * @brief  Write string - Sync
			 * @note   The settings of text like cursor position, font, size, color ,and etc is set by their dedicated functions.
			 * @param  *str: pointer (char*) to the string data
			 */

			void Write_Sync(char *str);

			void GetTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
			void SetSize(uint8_t s);
			void SetSize(uint8_t sx, uint8_t sy);
			void SetFont(const GfxFont_t *f = NULL);

			/**
			@brief  Set text cursor location
			@param  x    X coordinate in pixels
			@param  y    Y coordinate in pixels
		  */

			void SetCursor(int16_t x, int16_t y)
			{
				cursor_x = x;
				cursor_y = y;
			}

			/**
			@brief   Set text font color with transparant background
			@param   c   16-bit 5-6-5 Color to draw text with
			@note    For 'transparent' background, background and foreground
					 are set to same color rather than using a separate flag.
			*/
			void SetTextColor(Color_t c)
			{
				textcolor = textbgcolor = c;
			}

			/**
			@brief   Set text font color with custom background color
			@param   c   16-bit 5-6-5 Color to draw text with
			@param   bg  16-bit 5-6-5 Color to draw background/fill with
			*/
			void SetTextColor(Color_t c, Color_t bg)
			{
				textcolor = c;
				textbgcolor = bg;
			}

			/**
			@brief  Set whether text that is too long for the screen width should
					automatically wrap around to the next line (else clip right).
			@param  w  true for wrapping, false for clipping
			*/
			void SetTextWrap(bool w)
			{
				wrap = w;
			}

			/**
			@brief  Enable (or disable) Code Page 437-compatible charset.
					There was an error in glcdfont.c for the longest time -- one
					character (#176, the 'light shade' block) was missing -- this
					threw off the index of every character that followed it.
					But a TON of code has been written with the erroneous
					character indices. By default, the library uses the original
					'wrong' behavior and old sketches will still work. Pass
					'true' to this function to use correct CP437 character values
					in your code.
			@param  x  true = enable (new behavior), false = disable (old behavior)
			*/
			void SetCP437(bool x = true) { _cp437 = x; }

			/**
			@brief  Get text cursor X location
			@returns    X coordinate in pixels
			*/
			int16_t GetCursorX(void) const { return cursor_x; }

			/**
			@brief      Get text cursor Y location
			@returns    Y coordinate in pixels
			*/

			int16_t GetCursorY(void) const { return cursor_y; };

		protected:
			/**
			@brief  Helper to determine size of a character with current font/size.
					Broke this out as it's used by both the PROGMEM- and RAM-resident
					GetTextBounds() functions.
			@param  c     The ASCII character in question
			@param  x     Pointer to x location of character. Value is modified by
						this function to advance to next character.
			@param  y     Pointer to y location of character. Value is modified by
						this function to advance to next character.
			@param  minx  Pointer to minimum X coordinate, passed in to AND returned
						by this function -- this is used to incrementally build a
						bounding rectangle for a string.
			@param  miny  Pointer to minimum Y coord, passed in AND returned.
			@param  maxx  Pointer to maximum X coord, passed in AND returned.
			@param  maxy  Pointer to maximum Y coord, passed in AND returned.
			*/
			void GetCharBounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx,
							   int16_t *miny, int16_t *maxx, int16_t *maxy);
			int16_t cursor_x;	 ///< x location to start print()ing text
			int16_t cursor_y;	 ///< y location to start print()ing text
			Color_t textcolor;	 ///< 16-bit background color for print()
			Color_t textbgcolor; ///< 16-bit text color for print()
			uint8_t textsize_x;	 ///< Desired magnification in X-axis of text to print()
			uint8_t textsize_y;	 ///< Desired magnification in Y-axis of text to print()
			bool wrap;			 ///< If set, 'wrap' text at right edge of display
			bool _cp437;		 ///< If set, use correct CP437 charset (default is off)
			GfxFont_t *gfxFont;	 ///< Pointer to special font
		};

	public:
		Draw draw;
		Text text;

		// Constructor
		GFX(int16_t w, int16_t h) : Display_t(w, h), draw(*this), text(*this) {}
	};
}
