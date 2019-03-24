#ifndef _CONFIG_GAMEBUINO_META_
#define _CONFIG_GAMEBUINO_META_


#include <Arduino.h>


#define DISPLAY_LCD_WIDTH           320
#define DISPLAY_LCD_HEIGHT          240



#define DISPLAY_MODE_RGB565         0
#define DISPLAY_MODE_INDEX          1
#define DISPLAY_MODE_INDEX_HALFRES  2


#define DISPLAY_MODE                      DISPLAY_MODE_RGB565
#define DISPLAY_DEFAULT_BACKGROUND_COLOR  Color::black
#define DISPLAY_DEFAULT_COLOR             Color::white



#define DISPLAY_CONSTRUCTOR Image(DISPLAY_LCD_WIDTH/2, DISPLAY_LCD_HEIGHT/2, ColorMode::rgb565)


#define DEFAULT_FONT_SIZE 			  1
#define SYSTEM_DEFAULT_FONT_SIZE  1





#endif
