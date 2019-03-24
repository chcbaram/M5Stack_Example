#ifndef _TFT_LCD_H_
#define _TFT_LCD_H_


#include "Image.h"

#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
typedef unsigned char prog_uchar;



namespace Gamebuino_Meta {


class DisplayLcd : public Graphics {
public:
  DisplayLcd();


	void _drawPixel(int16_t x, int16_t y);
	void 	drawFastVLine(int16_t x, int16_t y, int16_t h);
	void	drawFastHLine(int16_t x, int16_t y, int16_t w);
	void	drawBufferedLine(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, Image& img);
	void  drawBuffer(int16_t x, int16_t y, uint16_t *buffer, uint16_t w, uint16_t h);
	void  sendBuffer(uint16_t *buffer, uint16_t n);
	void  drawImage(int16_t x, int16_t y, Image& img);
	void  drawImage(int16_t x, int16_t y, Image& img, int16_t w2, int16_t h2);
	void  fillRect(int16_t x, int16_t y, int16_t w, int16_t h);
	void  setRotation(uint8_t r);
	void  invertDisplay(boolean i);
	void  setScanline(bool enable);

	ColorMode colorMode = ColorMode::rgb565;

private:
	bool scanline_enable;
};

} // namespace Gamebuino_Meta

#endif
