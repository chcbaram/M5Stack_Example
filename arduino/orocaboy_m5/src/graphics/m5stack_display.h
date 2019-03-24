#pragma once

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif // min

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif // max



void ili9341_init();
void ili9341_write_frame(uint16_t* buffer);
void ili9341_write_frame_rectangle(short left, short top, short width, short height, uint16_t* buffer);
void ili9341_write_frame_rectangleLE(short left, short top, short width, short height, uint16_t* buffer);

void ili9341_clear(uint16_t color);

void send_reset_drawing(int left, int top, int width, int height);
void send_continue_line(uint16_t *line, int width, int lineCount);