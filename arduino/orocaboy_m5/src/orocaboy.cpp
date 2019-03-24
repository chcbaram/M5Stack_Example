/*
This file is part of the Gamebuino-Meta library,
Copyright (c) Aadalie 2014, 2016-2017

This is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

This is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License (LGPL) for more details.

You should have received a copy of the GNU Lesser General Public
License (LGPL) along with the library.
If not, see <http://www.gnu.org/licenses/>.

Authors:
 - Aurelien Rodot
 - Sorunome
*/

#include "orocaboy.h"



void drvLcdBufferClear(uint32_t rgb_code)
{
  ili9341_clear(rgb_code);
}


// a 3x5 font table
extern const uint8_t font3x5[];

namespace Gamebuino_Meta {

long random( long howbig )
{
  if ( howbig == 0 )
  {
    return 0 ;
  }

  return rand() % howbig;
}

long random( long howsmall, long howbig )
{
  if (howsmall >= howbig)
  {
    return howsmall;
  }

  long diff = howbig - howsmall;

  return random(diff) + howsmall;
}



	




const uint16_t startLightsData[] = {
	2, 4,
	24,
	1,
	0,
	0,
	
	0xc000,0x4000,0x4000,0x1000,0x1000,0x0,0x0,0x0,0x3800,0xa001,0x1000,0x3800,0x0,0x1000,0x0,0x0,0x1000,0x3001,0x0,0x9006,0x0,0x3003,0x0,0x1001,0x0,0x1001,0x0,0x3806,0x0,0xa815,0x1002,0x4008,0x0,0x1003,0x0,0x1003,0x1804,0x5810,0x5814,0xa03f,0x0,0x0,0x1809,0x0,0x501f,0x180d,0x70ff,0x403f,0x1818,0x0,0x305f,0x0,0x69ff,0x0,0x289f,0x101f,0x28bf,0x83f,0x7b5f,0x0,0x20ff,0x0,0x85f,0x0,0x94bf,0x295f,0x299f,0x87f,0x7f,0x0,0x0,0x0,0x221f,0xadff,0xdf,0x1a5f,0x0,0xfe,0x0,0x0,0x153,0xb7f,0x0,0xcf5f,0x0,0x49f,0x0,0x18b,0x0,0x1a9,0x0,0x63d,0x0,0xdfff,0x1c7,0x659,0x0,0x1c6,0x0,0x1c5,0x1c5,0x632,0x62f,0xa7fd,0x0,0x0,0x1a3,0x0,0x5e8,0x1a2,0x6ff7,0x5c6,0x1a1,0x0,0x5c3,0x0,0x4fef,0x0,0x5a1,0x180,0x580,0x180,0x2fe8,0x0,0x540,0x0,0x160,0x0,0x1fe3,0x4e0,0x4a0,0x140,0x120,0x0,0x0,0x0,0x2e0,0xfa0,0xa0,0x260,0x0,0xa0,0x0,0x0,0x60,0x960,0x0,0x1b60,0x0,0x900,0x0,0x40,0x0,0x40,0x0,0x8a0,0x0,0x2160,0x20,0x880,0x0,0x20,0x0,0x20,0x20,0x840,0x840,0x18c0,0x0,0x0,0x20,0x0,0x840,0x20,0x2060,0x1020,0x0,0x0,0x1020,0x0,0x3840,0x0,0x1820,0x800,0x2000,0x800,0x6820,0x0,0x2800,0x0,0x1000,0x0
};

const uint16_t buttonsIconsData[] = {
  10, 9,
  8,
  0,
  0,
  1, // color mode INDEX

  0x7705,0x7777,0x5750,0x3377,0x7577,0x7357,0x3777,0x5775,0x3373,0x7537,0x7357,0x3777,0x5775,0x7773,0x7537,0x7753,0x7777,0x5535,0x3333,0x5533,0x5505,0x5555,0x0550,0x5555,0x5055,0x7755,0x7777,0x5755,0x3377,0x7577,0x7357,0x3777,0x5775,0x3373,0x7537,0x7357,0x3777,0x5775,0x7773,0x7537,0x7755,0x7777,0x0555,0x5555,0x5055,0x7705,0x7777,0x5750,0x8878,0x7577,0x7857,0x8777,0x5775,0x8878,0x7577,0x7857,0x8777,0x5775,0x8878,0x7577,0x7758,0x7777,0x5585,0x8888,0x5588,0x5505,0x5555,0x0550,0x5555,0x5055,0x7755,0x7777,0x5755,0x8878,0x7577,0x7857,0x8777,0x5775,0x8878,0x7577,0x7857,0x8777,0x5775,0x8878,0x7577,0x7755,0x7777,0x0555,0x5555,0x5055,0x7705,0x7777,0x5750,0x4474,0x7547,0x7757,0x7777,0x5775,0x4474,0x7547,0x7757,0x7777,0x5775,0x4474,0x7547,0x7754,0x7777,0x5545,0x4444,0x5544,0x5505,0x5555,0x0550,0x5555,0x5055,0x7755,0x7777,0x5755,0x4474,0x7547,0x7757,0x7777,0x5775,0x4474,0x7547,0x7757,0x7777,0x5775,0x4474,0x7547,0x7755,0x7777,0x0555,0x5555,0x5055,0x7705,0x7777,0x5750,0x4477,0x7577,0x7457,0x4744,0x5775,0x4444,0x7544,0x7457,0x4747,0x5775,0x4774,0x7547,0x7754,0x7777,0x5545,0x4444,0x5544,0x5505,0x5555,0x0550,0x5555,0x5055,0x7755,0x7777,0x5755,0x4477,0x7577,0x7457,0x4744,0x5775,0x4444,0x7544,0x7457,0x4747,0x5775,0x4774,0x7547,0x7755,0x7777,0x0555,0x5555,0x5055
};

const uint16_t arrowsIconsData[] = {
  7,7,    // width, height
  8,      // number of frames
  0,      // frame looping speed
  0xCA30, // transparent color magenta
  0,      // color mode RGB
  //image data
  0xca30,0x5268,0xffff,0xffff,0xffff,0x5268,0xca30,0x5268,0xffff,0xffff,0xffff,0xffff,0xffff,0x5268,0x5268,0xffff,0x4439,0xffff,0x4439,0xffff,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0x4439,0xffff,0xffff,0xffff,0x4439,0x5268,0x5268,0x5268,0x4439,0x4439,0x4439,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0xffff,0xffff,0xffff,0x5268,0x5268,0xffff,0x4439,0xffff,0x4439,0xffff,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0xffff,0xffff,0xffff,0x5268,0xca30,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0xffff,0x4439,0xffff,0xffff,0xffff,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0x4439,0xffff,0xffff,0xffff,0x4439,0x5268,0x5268,0x5268,0x4439,0x4439,0x4439,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0xffff,0x4439,0xffff,0xffff,0xffff,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0xffff,0xffff,0xffff,0x5268,0xca30,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0xffff,0xffff,0xffff,0x4439,0xffff,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0x4439,0xffff,0xffff,0xffff,0x4439,0x5268,0x5268,0x5268,0x4439,0x4439,0x4439,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0xffff,0xffff,0xffff,0x4439,0xffff,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0xffff,0xffff,0xffff,0x5268,0xca30,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0xffff,0x4439,0xffff,0x4439,0xffff,0x5268,0x5268,0xffff,0xffff,0xffff,0xffff,0xffff,0x5268,0x5268,0x4439,0xffff,0xffff,0xffff,0x4439,0x5268,0x5268,0x5268,0x4439,0x4439,0x4439,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0x4439,0xffff,0xffff,0x5268,0x5268,0xffff,0x4439,0xffff,0x4439,0xffff,0x5268,0x5268,0xffff,0xffff,0xffff,0xffff,0xffff,0x5268,0x5268,0x5268,0xffff,0xffff,0xffff,0x5268,0x5268,0xca30,0x5268,0x5268,0x5268,0x5268,0x5268,0xca30
};

const uint8_t gamebuinoLogo[] = {80,10,
	0b00111100,0b00111111,0b00111111,0b11110011,0b11110011,0b11110011,0b00110011,0b00111111,0b00111111,0b00011100,
	0b00111100,0b00111111,0b00111111,0b11110011,0b11110011,0b11110011,0b00110011,0b00111111,0b00111111,0b00100110,
	0b00110000,0b00110011,0b00110011,0b00110011,0b00000011,0b00110011,0b00110011,0b00110011,0b00110011,0b00100110,
	0b00110000,0b00110011,0b00110011,0b00110011,0b00000011,0b00110011,0b00110011,0b00110011,0b00110011,0b00101010,
	0b00110011,0b00111111,0b00110011,0b00110011,0b11110011,0b11000011,0b00110011,0b00110011,0b00110011,0b00011100,
	0b00110011,0b00111111,0b00110011,0b00110011,0b11110011,0b11000011,0b00110011,0b00110011,0b00110011,0b00000000,
	0b00110011,0b00110011,0b00110011,0b00110011,0b00000011,0b00110011,0b00110011,0b00110011,0b00110011,0b00000000,
	0b00110011,0b00110011,0b00110011,0b00110011,0b00000011,0b00110011,0b00110011,0b00110011,0b00110011,0b00000000,
	0b00111111,0b00110011,0b00110011,0b00110011,0b11110011,0b11110011,0b11110011,0b00110011,0b00111111,0b00000000,
	0b00111111,0b00110011,0b00110011,0b00110011,0b11110011,0b11110011,0b11110011,0b00110011,0b00111111,0b00000000,
};


struct RGB888 {
  uint8_t r, g, b;
};

uint16_t rgb888Torgb565(RGB888 c) {
  return ((c.r & 0xF8) << 8) | ((c.g & 0xFC) << 3) | (c.b >> 3);
}

RGB888 rgb565Torgb888(uint16_t c) {
  RGB888 out;

  out.r = (uint8_t)((c >> 8) & 0xF8);
  out.r |= out.r >> 5;

  out.g = (uint8_t)((c >> 3) & 0xFC);
  out.g |= out.g >> 6;

  out.b = (uint8_t)(c << 3);
  out.b |= out.b >> 5;
  return out;
}

void Gamebuino::begin() {
	
	timePerFrame = 40; //25 FPS
	//nextFrameMillis = 0;
	//frameCount = 0;
	frameEndMicros = 1;
	startMenuTimer = 255;

	
  //buttons
  buttons.begin();
  buttons.update();

	//tft
  ili9341_init();  
  drvLcdBufferClear(0);

	display.fill(Color::black);
	display.fontSize = SYSTEM_DEFAULT_FONT_SIZE;
	

	display.setColor(Color::white, Color::black);
	display.fill(Color::black);
	
	display.clear();
}

void Gamebuino::drawLogo(int8_t x, int8_t y) {
	Image logo(78, 10, ColorMode::index);
	logo.clear();
	logo.drawBitmap(-2, 0, gamebuinoLogo);
	display.drawImage(x*display.fontSize, y*display.fontSize, logo, 78*display.fontSize, 10*display.fontSize);
}

void Gamebuino::startScreen(){
	Image startLights(startLightsData);
	int8_t i = 24;
	update();
	display.fontSize = SYSTEM_DEFAULT_FONT_SIZE;
	while(i){
		if(update()){
			i--;
			display.clear();
			Image logo(78, 10, ColorMode::index);
			drawLogo(2, min(0, i-10));
			lights.drawImage(0, 0, startLights);
		}
	}
	lights.clear();
	update();
}

void Gamebuino::titleScreen() {
  ColorMode ts_backup_colorMode = display.colorMode;
  uint16_t ts_backup_width = display._width;
  uint16_t ts_backup_height = display._height;
  display.fontSize = SYSTEM_DEFAULT_FONT_SIZE;
  char filename[17] = "TITLESCREEN.BMP";

  bool titleScreenImageExists = false;
  bool displayName = !titleScreenImageExists;

  bool first = true;
  bool reInitDisplay = false;

  Image buttonsIcons = Image(buttonsIconsData);

  while(1) {
    if(!update()) {
      continue;
    }

    if (titleScreenImageExists) {
      display.nextFrame();
    } else {
      display.clear();
    }

    if (displayName) {
      // center bar
      display.setColor(BROWN);
      display.fillRect(0, 15*display.fontSize, 80*display.fontSize, 9*display.fontSize);
      display.setColor(DARKGRAY);
      display.drawFastHLine(0, 14*display.fontSize, 80*display.fontSize);
      display.drawFastHLine(0, 24*display.fontSize, 80*display.fontSize);
      if (display.fontSize > 1) {
        display.drawFastHLine(0, 14*display.fontSize + 1, 80*display.fontSize);
        display.drawFastHLine(0, 24*display.fontSize + 1, 80*display.fontSize);
      }

      // game name
      display.setColor(WHITE);
      display.setCursor(2*display.fontSize, 17*display.fontSize);
      display.println("title");
    }

    //blinking A button icon
    if((gb.frameCount%8) < 4){
      buttonsIcons.setFrame(1); //button A pressed
    } else {
      buttonsIcons.setFrame(0); //button A released
    }
    uint8_t scale = gb.display.width() == 80 ? 1 : 2;
    uint8_t w = buttonsIcons.width() * scale;
    uint8_t h = buttonsIcons.height() * scale;
    uint8_t x = gb.display.width() - w - (2 * scale);
    uint8_t y = gb.display.height() - h - (2 * scale);
    gb.display.drawImage(x, y, buttonsIcons, w, h);

    break;
  }
  updateDisplay();
  delay(1000);

  display.init(ts_backup_width, ts_backup_height, ts_backup_colorMode);
}

void Gamebuino::titleScreen(const char* name, const char* logo) {
  display.fontSize = 1;
  //display.persistence = false;

  const char* msg = "\x15 to start";
  uint8_t w = display.fontWidth*strlen(msg)*display.fontSize;
  uint8_t h = display.fontHeight*display.fontSize;
  uint8_t x = (display.width() - w) / 2;
  uint8_t y = (display.height() / 5) * 3 + h;
  while(1) {
    if (!update()) {
      continue;
    }
    display.clear();
    display.setColor(Color::black);
    display.drawBitmap(-2, 0, gamebuinoLogo);
    if (logo){
      uint8_t lw = logo[0];
      uint8_t lh = logo[1];
      display.drawBitmap((display.width() - lw) / 2, (display.height() - lh) / 2, (uint8_t *)logo);
    }
    display.setCursor(0, 12);
    display.print(name);

    if ((frameCount % 32) < 20) {

      display.setColor(Color::gray);
      display.drawRect(x - display.fontSize*2, y - display.fontSize*2, w + display.fontSize*4, h + display.fontSize*3);

      display.setColor(Color::brown);
      display.fillRect(x - display.fontSize, y - display.fontSize, w + display.fontSize*2, h + display.fontSize);
      display.setColor(Color::white);
      display.setCursor(x, y);
      display.print(msg);
    }

    break;
  }

  updateDisplay();
  delay(1000);
}

void Gamebuino::titleScreen(const char* logo) {
  titleScreen("", logo);
}


bool recording_screen = false;

bool Gamebuino::update(bool draw_enable) {
	if (((nextFrameMillis - millis()) > timePerFrame) && frameEndMicros) { //if time to render a new frame is reached and the frame end has ran once
		nextFrameMillis = millis() + timePerFrame;
		frameCount++;

		frameEndMicros = 0;
		frameStartMicros = micros();

		buttons.update();

		return true;

	}
	if (frameEndMicros) {
		return false;
	}
	// ok, here is the first time after a frame, so we'll better check stuff correctly
	
	
	//draw and update popups
	updatePopup();
	
	
	
	//send buffer to the screen
  frameDrawTimePre = micros();
	if (draw_enable == true)
	{
	  updateDisplay();
	}
  frameDrawTime = micros() - frameDrawTimePre;

	frameEndMicros = micros(); //measure the frame's end time
	frameDurationMicros = frameEndMicros - frameStartMicros;
	return false;
}

void Gamebuino::updateDisplay() {
	tft.drawImage(0, 0, display, tft.width(), tft.height()); //send the buffer to the screen
}

void Gamebuino::setFrameRate(uint8_t fps) {
	timePerFrame = 1000 / fps;
}

void Gamebuino::pickRandomSeed(){

}

uint8_t Gamebuino::getCpuLoad(){
	return(frameDurationMicros/(10*timePerFrame));
}

uint32_t Gamebuino::getDrawFrameTime(){
	return frameDrawTime;
}
extern "C" char* sbrk(int incr);	
uint16_t Gamebuino::getFreeRam() {	
	// from https://github.com/mpflaga/Arduino-MemoryFree/blob/master/MemoryFree.cpp
	char top;
	return &top - reinterpret_cast<char*>(sbrk(0));
}

int8_t Gamebuino::menu(const char* const* items, uint8_t length) {

  return 0;
}



void Gamebuino::checkHomeMenu() {
}

void Hook_ExitHomeMenu() __attribute__((weak));
void Hook_ExitHomeMenu() {
	
}

bool homeMenuGetUniquePath(char* name, uint8_t offset, uint8_t len) {
	return true;
}

template<uint8_t N>
void fileEndingGmvToBmp(char (&name)[N]) {
	name[N-4] = 'B';
	//name[N-3] = 'M'; // no need as GMV and BMP have same second letter
	name[N-2] = 'P';
}

void Gamebuino::homeMenu(){
}

#define KEYBOARD_W 16
#define KEYBOARD_H 14
void Gamebuino::keyboard(char* text, uint8_t length) {

}

void Gamebuino::popup(const char* text, uint8_t duration){
	popupText = text;
	popupTimeLeft = duration+12;
}

void Gamebuino::updatePopup(){
	if (popupTimeLeft){
		uint8_t yOffset = 0;
		if(popupTimeLeft<12){
			yOffset = 12-popupTimeLeft;
		}
		display.setFontSize(1);
		display.setColor(DISPLAY_DEFAULT_BACKGROUND_COLOR);
		display.fillRoundRect(0,display.height()-display.getFontHeight()+yOffset-3,display.width(),display.getFontHeight()+3,3);
		display.setColor(DISPLAY_DEFAULT_COLOR);
		display.drawRoundRect(0,display.height()-display.getFontHeight()+yOffset-3,display.width(),display.getFontHeight()+3,3);
		display.setCursor(
			4,
			display.height()-display.getFontHeight()+yOffset-1
		);
		display.print(popupText);
		popupTimeLeft--;
	}
}

void Gamebuino::changeGame(){
	//clear the screen
	tft.fill(Color::black);
	//flash loader.bin
}

void Gamebuino::getDefaultName(char* string){
}

bool Gamebuino::collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h){
	if((x1>=x2)&&(x1<x2+w))
	if((y1>=y2)&&(y1<y2+h))
	return true;
	return false;
}

bool Gamebuino::collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2){
  return !( x2     >=  x1+w1  || 
            x2+w2  <=  x1     || 
            y2     >=  y1+h1  ||
            y2+h2  <=  y1     );
}

bool Gamebuino::collideBitmapBitmap(int16_t x1, int16_t y1, const uint8_t* b1, int16_t x2, int16_t y2, const uint8_t* b2){
  int16_t w1 = b1[0];
  int16_t h1 = b1[1];
  int16_t w2 = b2[0];
  int16_t h2 = b2[1];

  if(collideRectRect(x1, y1, w1, h1, x2, y2, w2, h2) == false){
    return false;
  }
  
  int16_t xmin = (x1>=x2)? 0 : x2-x1;
  int16_t ymin = (y1>=y2)? 0 : y2-y1;
  int16_t xmax = (x1+w1>=x2+w2)? x2+w2-x1 : w1;
  int16_t ymax = (y1+h1>=y2+h2)? y2+h2-y1 : h1;

  for(uint8_t y = ymin; y < ymax; y++){
    for(uint8_t x = xmin; x < xmax; x++){
      if(display.getBitmapPixel(b1, x, y) && display.getBitmapPixel(b2, x1+x-x2, y1+y-y2)){
        return true;
      }
    }
  }

  return false;
}

Color Gamebuino::createColor(uint8_t r, uint8_t g, uint8_t b) {
	return (Color)rgb888Torgb565({r, g, b});
}

uint8_t Gamebuino::getTimePerFrame() {
	return timePerFrame;
}

int8_t tone_identifier = -1;

} // namespace Gamebuino_Meta

#ifndef GAMEBUINO_COMPAT_MODE
Gamebuino gb;
#endif
/*
void tone(uint32_t outputPin, uint32_t frequency, uint32_t duration) {
}


void noTone(uint32_t outputPin) {
}

void yield() {
	if (gb.inited && (gb.frameEndMicros || gb.frameStartMicros)) {
		gb.update();
	}
}
*/