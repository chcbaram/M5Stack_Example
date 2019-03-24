/***************************************************
	This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
	----> http://www.adafruit.com/products/358
The 1.8" TFT shield
	----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
	----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
	----> http://www.adafruit.com/products/618

	Check out the links above for our tutorials and wiring diagrams
	These displays use SPI to communicate, 4 or 5 pins are required to
	interface (RST is optional)
	Adafruit invests time and resources providing this open source code,
	please support Adafruit and open-source hardware by purchasing
	products from Adafruit!

	Written by Limor Fried/Ladyada for Adafruit Industries.
	MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <limits.h>
#include "DisplayLcd.h"
#include "Image.h"
#include "Graphics.h"
//#include "lcd.h"
//#include "driver/drv_lcd.h"
#include "config.h"

uint32_t draw_time;
namespace Gamebuino_Meta {




inline uint16_t swapcolor(uint16_t x) { 
	return (x << 11) | (x & 0x07E0) | (x >> 11);
}

void drvLcdCopyLineBuffer(uint16_t x_pos, uint16_t y_pos, uint8_t *p_data, uint32_t length)
{

}



DisplayLcd::DisplayLcd() : Graphics(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT)
{
  scanline_enable = false;
}



//fast method to quickly push a buffered line of pixels
//boundary check must be made prior to this function
//the color must be formated as the destination
void DisplayLcd::drawBufferedLine(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, Image& img)
{
  //st7735drawBufferedLine(x, y, buffer, w);
}

//fast method to quickly push a buffered line of pixels
//boundary check must be made prior to this function
//the color must be formated as the destination
void DisplayLcd::drawBuffer(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, uint16_t h)
{
  uint16_t preBufferLineArray[w*2];
  uint16_t *preBufferLine = preBufferLineArray;


  for (uint16_t i = 0; i < w; i++)
  {
    uint16_t color = buffer[i];
    preBufferLine[i * 2] = preBufferLine[(i * 2) + 1] = color;
  }

  drvLcdCopyLineBuffer((uint16_t)(x*2), (uint16_t)(y*2), (uint8_t *)preBufferLine, w*2);
}

//fast method to quickly push a buffered line of pixels
//boundary check must be made prior to this function
//the color must be formated as the destination
void DisplayLcd::sendBuffer(uint16_t *buffer, uint16_t n)
{
  //st7735sendBuffer(buffer, n);
}

uint16_t swap_endians_16(uint16_t b)
{
	return (b << 8) | (b >> 8);
}


void DisplayLcd::drawImage(int16_t x, int16_t y, Image& img)
{
	img.nextFrame();
	int16_t w = img._width;
	int16_t h = img._height;
	if (w == 0 || h == 0) {
		return;
	}

	if ((img.colorMode == ColorMode::index) && (w == _width) && (h == _height)) {

		uint16_t preBufferLineArray[w];
		uint16_t sendBufferLineArray[w];
		uint16_t *preBufferLine = preBufferLineArray;
		uint16_t *sendBufferLine = sendBufferLineArray;

		//set the window to the whole screen
		//st7735SetAddrWindow(0, 0, _width - 1, _height - 1);


		//prepare the first line
		indexTo565(preBufferLine, (uint8_t*)img._buffer, Graphics::colorIndex, w, false);
		for (uint16_t i = 0; i < w; i++) { //horizontal coordinate in source image
			uint16_t color = preBufferLine[i];
			preBufferLine[i] = swap_endians_16(color);
		}

		//start sending lines and processing them in parallel using DMA
		for (uint16_t j = 1; j < h; j++) { //vertical coordinate in source image, start from the second line

			//swap buffers pointers
			uint16_t *temp = preBufferLine;
			preBufferLine = sendBufferLine;
			sendBufferLine = temp;
			

			//st7735WritePin(_PIN_DEF_DC, _DEF_HIGH);
			//st7735WritePin(_PIN_DEF_CS, _DEF_LOW);

			sendBuffer(sendBufferLine, w); //start DMA send


			//prepare the next line while the current one is being transferred

			//length is the number of destination pixels
			uint16_t *dest = preBufferLine;
			uint16_t *src = img._buffer + ((j * w) / 4);
			Color *index = Graphics::colorIndex;
			uint16_t length = w;
			for (uint16_t i = 0; i < length / 4; i++) {
				uint16_t index1 = (src[i] >> 4) & 0x000F;
				uint16_t index2 = (src[i] >> 0) & 0x000F;
				uint16_t index3 = (src[i] >> 12) & 0x000F;
				uint16_t index4 = (src[i] >> 8) & 0x000F;
				//change pixel order (because of words endianness) at the same time
				dest[i * 4] = swap_endians_16((uint16_t)index[index1]);
				dest[(i * 4) + 1] = swap_endians_16((uint16_t)index[index2]);
				dest[(i * 4) + 2] = swap_endians_16((uint16_t)index[index3]);
				dest[(i * 4) + 3] = swap_endians_16((uint16_t)index[index4]);
			}

			//while (!spiDmaIsTxDone(_DEF_SPI1)); //chill

			//st7735WritePin(_PIN_DEF_CS, _DEF_HIGH);
		}

		//send the last line
    //st7735WritePin(_PIN_DEF_DC, _DEF_HIGH);
    //st7735WritePin(_PIN_DEF_CS, _DEF_LOW);

    //sendBuffer(preBufferLine, w); //start DMA send
    //while (!spiDmaIsTxDone(_DEF_SPI1)); //chill

    //st7735WritePin(_PIN_DEF_CS, _DEF_HIGH);

    return;
	}
	
	Graphics::drawImage(x, y, img); //fallback to the usual
}

void bufferIndexLineDouble(uint16_t* preBufferLine, uint16_t* img_buffer, int16_t w, uint16_t j) {
	int16_t w2 = w*2;
	uint16_t *dest = preBufferLine;
	uint16_t *src = img_buffer + ((j * w) / 4);
	Color *index = Graphics::colorIndex;
	uint16_t length = w;
	for (uint16_t i = 0; i < length / 4; i++) {
		uint16_t index1 = (src[i] >> 4) & 0x000F;
		uint16_t index2 = (src[i] >> 0) & 0x000F;
		uint16_t index3 = (src[i] >> 12) & 0x000F;
		uint16_t index4 = (src[i] >> 8) & 0x000F;
		//change pixel order (because of words endianness) at the same time
		// endians are swapped because SPI sends byte-by-byte instead of word-by-word
		*(dest++) = swap_endians_16((uint16_t)index[index1]);
		*(dest++) = swap_endians_16((uint16_t)index[index1]);
		*(dest++) = swap_endians_16((uint16_t)index[index2]);
		*(dest++) = swap_endians_16((uint16_t)index[index2]);
		*(dest++) = swap_endians_16((uint16_t)index[index3]);
		*(dest++) = swap_endians_16((uint16_t)index[index3]);
		*(dest++) = swap_endians_16((uint16_t)index[index4]);
		*(dest++) = swap_endians_16((uint16_t)index[index4]);
	}
	
	memcpy(&preBufferLine[w2], preBufferLine, w2 * 2); //double the line on the second half of the buffer
}





void DisplayLcd::drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2) {
	img.nextFrame();
	//out of screen
	if ((x > _width) || ((x + abs(w2)) < 0) || (y > _height) || ((y + abs(h2)) < 0) || (w2 == 0) || (h2 == 0)) return;

	int16_t w = img._width;
	int16_t h = img._height;
	if (w == 0 || h == 0) {
		return;
	}

	//no scaling
	if ((w == w2) && (h == h2)) {
		drawImage(x, y, img);
		return;
	}


	//x2 upscaling to full screen
	if ((w2 == (w * 2)) && (h2 == (h * 2)) && (_width == w2) && (_height == h2))
	{
		if (img.colorMode == ColorMode::rgb565)
		{
			uint16_t preBufferLineArray[w2 * 2];
			uint16_t sendBufferLineArray[w2 * 2];
			uint16_t *preBufferLine = preBufferLineArray;
			uint16_t *sendBufferLine = sendBufferLineArray;

			uint32_t pre_time;

			pre_time = micros();

      uint32_t line_count = 2;

      if (scanline_enable == true) line_count = 1;


			//prepare the first line
			for (uint16_t i = 0; i < w; i++) { //horizontal coordinate in source image
				uint16_t color = img._buffer[i];
				preBufferLine[i * 2] = preBufferLine[(i * 2) + 1] = swap_endians_16(color);
			}
			memcpy(&preBufferLine[w2], preBufferLine, w2 * line_count); //double the line on the second half of the buffer

			uint16_t line = 0;
      
			
      //while(lcdDrawAvailable() == false);
      send_reset_drawing(0, 0, _width, _height);


			//start sending lines and processing them in parallel using DMA
			for (uint16_t j = 1; j < h; j ++) { //vertical coordinate in source image, start from the second line

				//swap buffers pointers
				uint16_t *temp = preBufferLine;
				preBufferLine = sendBufferLine;
				sendBufferLine = temp;

				//drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*2), (uint8_t *)sendBufferLine, (uint32_t)(_width * line_count));
        send_continue_line(sendBufferLine, _width, line_count);

				line++;

				#if 1
				//prepare the next line while the current one is being transferred
				for (uint16_t i = 0; i < w; i ++) { //horizontal coordinate in source image
					uint16_t color = img._buffer[(j * w) + i];
					preBufferLine[i * 2] = preBufferLine[(i * 2) + 1] = swap_endians_16(color);
				}
				#else
				uint32_t *p_src = (uint32_t *)&img._buffer[(j * w) + 0];

        for (uint16_t i = 0; i < w/2; i ++) { //horizontal coordinate in source image
          uint32_t color_o = p_src[i];
          uint16_t color_l;
          uint16_t color_h;

          color_l = color_o;
          color_h = color_o>>16;

          color_l = swap_endians_16(color_l);
          color_h = swap_endians_16(color_h);

          preBufferLine[(i * 4) + 0] = color_l;
          preBufferLine[(i * 4) + 1] = color_l;
          preBufferLine[(i * 4) + 2] = color_h;
          preBufferLine[(i * 4) + 3] = color_h;
        }
        #endif


				memcpy(&preBufferLine[w2], preBufferLine, w2 * line_count); //double the line on the second half of the buffer
			}

      //drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*2), (uint8_t *)preBufferLine, (uint32_t)(_width * line_count));
      send_continue_line(preBufferLine, _width, line_count);

      line++;

      //lcdRequestDraw();

	    draw_time = micros() - pre_time;
			return;
		}
		if (img.colorMode == ColorMode::index) {
			uint16_t preBufferLineArray[w2 * 2];
			uint16_t sendBufferLineArray[w2 * 2];
			uint16_t *preBufferLine = preBufferLineArray;
			uint16_t *sendBufferLine = sendBufferLineArray;

			//set the window to the whole screen
			//st7735SetAddrWindow(0, 0, _width - 1, _height - 1);


			bufferIndexLineDouble(preBufferLine, img._buffer, w, 0);

			//start sending lines and processing them in parallel using DMA
			for (uint16_t j = 1; j < h; j++) { //vertical coordinate in source image, start from the second line

				//swap buffers pointers
				uint16_t *temp = preBufferLine;
				preBufferLine = sendBufferLine;
				sendBufferLine = temp;
				
        //st7735WritePin(_PIN_DEF_DC, _DEF_HIGH);
        //st7735WritePin(_PIN_DEF_CS, _DEF_LOW);

				sendBuffer(sendBufferLine, _width * 2); //start DMA send

				// prepare the next line while we'r at it
				bufferIndexLineDouble(preBufferLine, img._buffer, w, j);

        //while (!spiDmaIsTxDone(_DEF_SPI1)); //chill

        //st7735WritePin(_PIN_DEF_CS, _DEF_HIGH);
			}

			//send the last line
      //st7735WritePin(_PIN_DEF_DC, _DEF_HIGH);
      //st7735WritePin(_PIN_DEF_CS, _DEF_LOW);

      //sendBuffer(preBufferLine, _width * 2); //start DMA send
      //while (!spiDmaIsTxDone(_DEF_SPI1)); //chill

      //st7735WritePin(_PIN_DEF_CS, _DEF_HIGH);
			return;
		}
	}

  //x4 upscaling to full screen
  if ((w2 == (w * 4)) && (h2 == (h * 4)) && (_width == w2) && (_height == h2))
  {
    if (img.colorMode == ColorMode::rgb565)
    {
      uint16_t preBufferLineArray[w2];
      uint16_t sendBufferLineArray[w2];
      uint16_t *preBufferLine = preBufferLineArray;
      uint16_t *sendBufferLine = sendBufferLineArray;

      uint32_t pre_time;

      pre_time = micros();

      uint32_t line_count = 1;



      //prepare the first line
      for (uint16_t i = 0; i < w; i++) { //horizontal coordinate in source image
        uint16_t color = img._buffer[i];
        preBufferLine[(i * 4) + 0] = preBufferLine[(i * 4) + 1] = color;
        preBufferLine[(i * 4) + 2] = preBufferLine[(i * 4) + 3] = color;
      }

      uint16_t line = 0;

      //while(lcdDrawAvailable() == false);



      //start sending lines and processing them in parallel using DMA
      for (uint16_t j = 1; j < h; j ++) { //vertical coordinate in source image, start from the second line

        //swap buffers pointers
        uint16_t *temp = preBufferLine;
        preBufferLine = sendBufferLine;
        sendBufferLine = temp;

        drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 0), (uint8_t *)sendBufferLine, (uint32_t)(_width));
        //drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 1), (uint8_t *)sendBufferLine, (uint32_t)(_width));
        drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 2), (uint8_t *)sendBufferLine, (uint32_t)(_width));
        //drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 3), (uint8_t *)sendBufferLine, (uint32_t)(_width));
        line++;


        uint32_t *p_src = (uint32_t *)&img._buffer[(j * w) + 0];

        for (uint16_t i = 0; i < w/2; i ++) { //horizontal coordinate in source image
          uint32_t color = p_src[i];

          preBufferLine[(i * 8) + 0] = color;
          preBufferLine[(i * 8) + 1] = color;
          preBufferLine[(i * 8) + 2] = color;
          preBufferLine[(i * 8) + 3] = color;

          preBufferLine[(i * 8) + 4] = color>>16;
          preBufferLine[(i * 8) + 5] = color>>16;
          preBufferLine[(i * 8) + 6] = color>>16;
          preBufferLine[(i * 8) + 7] = color>>16;
        }
      }

      drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 0), (uint8_t *)preBufferLine, (uint32_t)(_width));
      drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 1), (uint8_t *)preBufferLine, (uint32_t)(_width));
      drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 2), (uint8_t *)preBufferLine, (uint32_t)(_width));
      drvLcdCopyLineBuffer((uint16_t)0, (uint16_t)(line*4 + 3), (uint8_t *)preBufferLine, (uint32_t)(_width));
      line++;

      //lcdRequestDraw();

      draw_time = micros() - pre_time;
      return;
    }
  }

	// fall back to most generic but slow resizing
	Graphics::drawImage(x, y, img, w2, h2);
}




void DisplayLcd::_drawPixel(int16_t x, int16_t y) {

	if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

	//st7735SetAddrWindow(x,y,x+1,y+1);

	
	//spiTransfer8(_DEF_SPI1, (uint16_t)color.c >> 8);
	//spiTransfer8(_DEF_SPI1, (uint16_t)color.c);

}


void DisplayLcd::drawFastVLine(int16_t x, int16_t y, int16_t h) {
	// Rudimentary clipping
	if((x >= _width) || (y >= _height)) return;
	if((y+h-1) >= _height) h = _height-y;

	//st7735SetAddrWindow(x, y, x, y+h-1);

	uint8_t hi = (uint16_t)Graphics::color.c >> 8, lo = (uint16_t)Graphics::color.c;
		


	while (h--)
	{
    //spiTransfer8(_DEF_SPI1, hi);
    //spiTransfer8(_DEF_SPI1, lo);
	}
}


void DisplayLcd::drawFastHLine(int16_t x, int16_t y, int16_t w) {
	// Rudimentary clipping
	if((x >= _width) || (y >= _height)) return;
	if((x+w-1) >= _width)	w = _width-x;

	//st7735SetAddrWindow(x, y, x+w-1, y);

	uint8_t hi = (uint16_t)Graphics::color.c >> 8, lo = (uint16_t)Graphics::color.c;


	while (w--)
	{
    //spiTransfer8(_DEF_SPI1, hi);
    //spiTransfer8(_DEF_SPI1, lo);
	}
}

// fill a rectangle
void DisplayLcd::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	// rudimentary clipping (drawChar w/big text requires this)
	if((x >= _width) || (y >= _height)) return;
	if((x + w - 1) >= _width)	w = _width	- x;
	if((y + h - 1) >= _height) h = _height - y;

	//st7735SetAddrWindow(x, y, x+w-1, y+h-1);

	uint8_t hi = (uint16_t)Graphics::color.c >> 8, lo = (uint16_t)Graphics::color.c;
		

	for(y=h; y>0; y--) {
		for(x=w; x>0; x--) {
		  //spiTransfer8(_DEF_SPI1, hi);
		  //spiTransfer8(_DEF_SPI1, lo);
		}
	}
}


void DisplayLcd::invertDisplay(boolean i)
{

}


void DisplayLcd::setScanline(bool enable)
{
  scanline_enable = enable;
}


}

