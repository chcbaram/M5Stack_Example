#include <M5Stack.h>
#include "menu.h"



#define MENU_MAX_NODE             32


typedef struct
{
  uint8_t index;
  char    str[16];
  uint8_t str_length;

  void (*setup_func)(void);
  void (*loop_func)(void);
} menu_node_t;


typedef struct 
{
  uint8_t run_count;  
  uint8_t count;
  uint8_t cursor;
  uint8_t pre_cursor;
  uint8_t first_rows;
  uint8_t first_rows_pre;
  uint8_t view_rows;
  uint32_t pre_time;
  uint32_t pre_time_pressed;
  uint32_t press_count;
  bool     press_state;
  menu_node_t node[MENU_MAX_NODE];
} menu_t;


menu_t menu;
menu_t *p_menu = &menu;

static bool update_lcd = true;

bool menuDraw(menu_t *p_menu);
int text_size = 2;
int font_size = 16;


void menuInit(void)
{
  p_menu->run_count = 0;
  p_menu->count = 0;
  p_menu->cursor = 0;
  p_menu->first_rows = 0;
  p_menu->first_rows_pre = 0;
  p_menu->view_rows = M5.Lcd.height()/(font_size*text_size);
  p_menu->pre_time = millis();
  p_menu->press_count = 0;
  p_menu->press_state = M5.BtnA.isPressed();   

  hanSetSize(text_size);
}


 void menuAdd(const char *menu_str, void (*setup_func)(void), void (*loop_func)(void))
{
  uint8_t index;

  index = p_menu->count++;

  strcpy(p_menu->node[index].str, menu_str);
  p_menu->node[index].setup_func = setup_func;
  p_menu->node[index].loop_func = loop_func;
}

void menuUpdate(void)
{
  if (p_menu->run_count > 0)
  {
    if (p_menu->run_count == 1)
    {
      p_menu->run_count++;
      if (p_menu->node[p_menu->cursor].setup_func != NULL)
      {
        p_menu->node[p_menu->cursor].setup_func();
      }
    }
    else
    {
      if (p_menu->node[p_menu->cursor].loop_func != NULL)
      {
        p_menu->node[p_menu->cursor].loop_func();        
      }
      else
      {
        p_menu->run_count = 0;
        p_menu->pre_time = millis();
        p_menu->press_count = 0;
        p_menu->press_state = M5.BtnA.isPressed();         
      }
    }
  }
  else
  {
    menuDraw(p_menu);
  }  
}

bool menuDraw(menu_t *p_menu)
{
  uint8_t view_rows;
  uint8_t index;
  uint8_t press_count = 0;
  bool press_done = false;


  p_menu->pre_cursor = p_menu->cursor;

  if (p_menu->count == 0)
  {
    return false;
  }  

  // 버튼 처리 
  if (M5.BtnB.wasReleased())
  {
    for (int i=0; i< M5.Lcd.width(); i+=2)
    {
      M5.Lcd.fillRect(i, 0, 2, M5.Lcd.height(), TFT_BLACK);
      M5.Lcd.fillRect(i+2, 0, 2, M5.Lcd.height(), TFT_BLUE);
      delay(5);
    }
    p_menu->run_count = 1;      
    return true;
  }
  
  

  // 커서 처리 
  if (M5.BtnC.wasReleased())
  {
    p_menu->cursor++;
    p_menu->cursor %= p_menu->count;            
  }
  if (M5.BtnA.wasReleased())
  {
    p_menu->cursor--;
    p_menu->cursor = constrain(p_menu->cursor, 0, p_menu->count - 1);
  }
  p_menu->first_rows_pre = p_menu->first_rows;
  p_menu->first_rows = (p_menu->cursor/p_menu->view_rows) * p_menu->view_rows;

  if (p_menu->first_rows != p_menu->first_rows_pre)
  {
    update_lcd = true;
  }

  view_rows = p_menu->count - p_menu->first_rows;
  view_rows = constrain(view_rows, 0, p_menu->view_rows);

  if (update_lcd == true)
  {
    M5.Lcd.clearDisplay();
    update_lcd = false;
  }

  for (int i=0; i<view_rows; i++)
  {
    index = p_menu->first_rows + i;
    hanPrintf(6, font_size*i*text_size + 1, "%02d %s", index+1, p_menu->node[index].str);


    if (index == p_menu->pre_cursor && p_menu->cursor != p_menu->pre_cursor)
    {
      M5.Lcd.drawRoundRect(0, font_size*i*text_size, M5.Lcd.width(), font_size*text_size, 2, TFT_BLACK);
    }
    if (index == p_menu->cursor)
    {
      M5.Lcd.drawRoundRect(0, font_size*i*text_size, M5.Lcd.width(), font_size*text_size, 2, TFT_YELLOW);
    }
  }

  return false;
}
