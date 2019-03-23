#include <M5Stack.h>
#include "./hangul/PHan_Lib.h"



static uint8_t text_size = 1;



static void drawHanFont(int x, int y, PHAN_FONT_OBJ *FontPtr);


void hanSetSize(uint8_t _text_size)
{
  text_size = _text_size;
}

void hanPrintf(int x, int y,  const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];
  int Str_Size;
  int Size_Char;
  int i, x_Pre = x;
  PHAN_FONT_OBJ FontBuf;


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  for( i=0; i<len; i+=Size_Char )
  {
    PHan_FontLoad( &print_buffer[i], &FontBuf );
      
      
    drawHanFont( x, y, &FontBuf );
                      
    Size_Char = FontBuf.Size_Char;
    if (Size_Char >= 2)
    {           
        x += (2*text_size)*8;
    }       
    else
    {
        x += (1*text_size)*8;
    }
    
    if( M5.Lcd.width() < x ) 
    {
        x  = x_Pre;
        y += 16 * text_size;
    }
    
    if( FontBuf.Code_Type == PHAN_END_CODE ) break;
  }         
}

static  void drawHanFont(int x, int y, PHAN_FONT_OBJ *FontPtr)
{
  uint16_t    i, j, Loop;
  uint16_t  FontSize = FontPtr->Size_Char;
  uint16_t index_x;
  uint32_t x_offset;
  uint32_t y_offset;

  if (FontSize > 2)
  {
    FontSize = 2;
  }

  for ( i = 0 ; i < 16 ; i++ )        // 16 Lines per Font/Char
  {
    index_x = 0;
    for ( j = 0 ; j < FontSize ; j++ )      // 16 x 16 (2 Bytes)
    {
      for( Loop=0; Loop<8; Loop++ )
      {
        if( FontPtr->FontBuffer[i*FontSize +j] & (0x80>>Loop))
        {
          if (text_size == 1)
          {
            M5.Lcd.drawPixel(x + index_x, y + i, M5.Lcd.textcolor);
          }
          else
          {
            x_offset = x + index_x*text_size;
            y_offset = y + i*text_size;
            M5.Lcd.drawRect(x_offset, y_offset, text_size, text_size, M5.Lcd.textcolor);
          }          
        } 
        index_x++;
      }
    }
  }
}