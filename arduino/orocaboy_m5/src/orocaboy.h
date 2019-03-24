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

#ifndef _GAMEBUINO_META_H_
#define	_GAMEBUINO_META_H_

#include "graphics/DisplayLcd.h"
#include "graphics/config.h"



#include "graphics/Graphics.h"
#include "graphics/Image.h"
#include "buttons/Buttons.h"
#include "sound/Sound.h"
#include "collide/Collide.h"




namespace Gamebuino_Meta {


#define wrap(i, imax) ((imax+i)%(imax))

class Gamebuino {
public:
  Buttons buttons;
  Sound   sound;

	Image display = DISPLAY_CONSTRUCTOR;
	DisplayLcd tft = DisplayLcd();
	Image lights = Image(2, 4, ColorMode::rgb565);
  Collide collide;

	void begin();
	void startScreen();
	void titleScreen();
	bool update(bool draw_enable = true);
	void updateDisplay();
	uint8_t startMenuTimer;
	uint32_t frameCount;
	void setFrameRate(uint8_t fps);
	void pickRandomSeed();
	
	uint8_t getCpuLoad();
	uint16_t getFreeRam();
  uint32_t getDrawFrameTime();
	uint32_t frameDurationMicros;
	uint32_t frameStartMicros, frameEndMicros;
  uint32_t frameDrawTime;
  uint32_t frameDrawTimePre;
	int8_t menu(const char* const* items, uint8_t length);
	void checkHomeMenu();
	void homeMenu();
	void keyboard(char* text, uint8_t length);
	void popup(const char* text, uint8_t duration);
	
	void changeGame();
	void getDefaultName(char* string);
	
	bool collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h);
	bool collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2);
	bool collideBitmapBitmap(int16_t x1, int16_t y1, const uint8_t* b1, int16_t x2, int16_t y2, const uint8_t* b2);
	
	Color createColor(uint8_t r, uint8_t g, uint8_t b);
	uint8_t getTimePerFrame();
	
	void titleScreen(const char* name, const char* logo);
	void titleScreen(const char* logo);

	// so that we know when the object is actually initialized
	// importent for proper detection of what gb.display.width() and gb.display.height() return outside any function
	// unfortunatelly the arduino platform.txt doesn't allow us to change the order of object files, which would be the prefered way
	// as the Gamebuino constructor would be called then first, for sure
	bool inited = true;

private:
	uint8_t timePerFrame;
	uint32_t nextFrameMillis;
	void updatePopup();
	void drawLogo(int8_t x, int8_t y);
	const char* popupText;
	uint8_t popupTimeLeft;
	int16_t framesDisplayRecording = -1;
};


} // namespace Gamebuino_Meta

using Gamebuino_Meta::Gamebuino;
extern Gamebuino gb;

long random( long howsmall, long howbig );

#endif	// _GAMEBUINO_META_H_
