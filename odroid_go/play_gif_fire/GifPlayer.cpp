#include "GifPlayer.h"
#include <odroid_go.h>
#include "./src/GifDecoder.h"


#define PLAY_FILE     0
#define PLAY_BUFF     1



#define LCD_WIDTH    320 
#define LCD_HEIGHT   240 

#define LCD_FRAMEBUFFER



GifDecoder<LCD_WIDTH, LCD_HEIGHT, 12> decoder;




typedef struct
{
  bool is_init;  
  bool is_busy;
  bool is_stop;
  bool is_opened;
  bool is_draw;

  uint8_t play_mode;
  char file_name[128];  
   

  uint8_t  *gif_buf;
  uint32_t  gif_length;

  uint8_t *play_buff;
  uint32_t play_buff_seek;
  uint32_t play_buff_length;
  int      play_cycle;
} gif_node_t;


static gif_node_t gif_play;
static uint16_t *frame = NULL;

volatile static SemaphoreHandle_t play_semaphore = NULL;
volatile static TaskHandle_t _play_task_handle = NULL;

volatile static SemaphoreHandle_t draw_semaphore = NULL;
volatile static TaskHandle_t _draw_task_handle = NULL;


static bool gifplayStartFile(char *p_name);
static bool gifplayStartBuff(uint8_t *p_buff, uint32_t length);

static bool fileSeekCallback(unsigned long position);
static unsigned long filePositionCallback(void);
static int fileReadCallback(void);
static int fileReadBlockCallback(void * buffer, int numberOfBytes);

static void screenClearCallback(void);
static void updateScreenCallback(void);
static void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue);
static void drawLineCallback(int16_t x, int16_t y, uint8_t *buf, int16_t w, uint16_t *palette, int16_t skip);



void playerThread(void *arg)
{
  GifPlayer *p_player = (GifPlayer *)arg;

  while(1)
  {
    if (xSemaphoreTake(play_semaphore, 10) == pdTRUE)
    {
      gif_play.is_busy = true;
      if (gif_play.play_mode == PLAY_FILE)
      {
        gifplayStartFile(gif_play.file_name);
      }
      if (gif_play.play_mode == PLAY_BUFF)
      {
        gifplayStartBuff(gif_play.play_buff, gif_play.play_buff_length);
      }      
      gif_play.is_busy = false;
    }      
    delay(1);
  }
}

void drawThread(void *arg)
{
  while(1)
  {
    if (xSemaphoreTake(draw_semaphore, 100) == pdTRUE)
    {
      gif_play.is_draw = true;
      GO.lcd.setWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
      GO.lcd.pushColors((uint8_t *)frame, LCD_WIDTH * LCD_HEIGHT * 2);      
      gif_play.is_draw = false;
    }      
    delay(1);
  }
}

GifPlayer::GifPlayer(void)
{
  gif_play.is_init = false;
  gif_play.is_opened = false;
  gif_play.is_stop = false;
  gif_play.is_draw = false;

  gif_play.gif_buf = NULL;
  gif_play.gif_length = 0;
  gif_play.play_buff = NULL;
  gif_play.play_buff_seek = 0;
  gif_play.play_cycle = 1;

}

GifPlayer::~GifPlayer()
{
	
}

bool GifPlayer::isInit(void)
{
  return gif_play.is_init;
}

bool GifPlayer::begin(void)
{  
  if (gif_play.is_init == false)
  {  
    if (psramFound() != true)
    {
      psramInit();
      if (psramFound() != true)
      {
        Serial.println("No PSRAM");
      }
    }

    #ifdef LCD_FRAMEBUFFER
    frame = (uint16_t *)ps_malloc(LCD_WIDTH * LCD_HEIGHT * 2);
    if (frame == NULL)
    {
      Serial.println("fail to malloc for frame");
    }
    #endif

    if (play_semaphore == NULL)
    {
      play_semaphore = xSemaphoreCreateBinary();
    }
    if (draw_semaphore == NULL)
    {
      draw_semaphore = xSemaphoreCreateBinary();
    }    
    xTaskCreate(playerThread, "playerThread", 4096, this, 1, (TaskHandle_t*)&_play_task_handle);    
    xTaskCreate(drawThread, "drawThread", 4096, this, 1, (TaskHandle_t*)&_draw_task_handle);    
    

    if (!_play_task_handle || !_draw_task_handle)
    {
      gif_play.is_init = false;
    }
    else
    {
      gif_play.is_init = true;
    }    
    
    if(!SPIFFS.begin(true))
    {
      gif_play.is_init = false;    
      return false;
    }  

    decoder.setScreenClearCallback(screenClearCallback);
    decoder.setUpdateScreenCallback(updateScreenCallback);
    decoder.setDrawPixelCallback(drawPixelCallback);
    decoder.setDrawLineCallback(drawLineCallback);

    decoder.setFileSeekCallback(fileSeekCallback);
    decoder.setFilePositionCallback(filePositionCallback);
    decoder.setFileReadCallback(fileReadCallback);
    decoder.setFileReadBlockCallback(fileReadBlockCallback);
  }
  

  return gif_play.is_init;
}

bool GifPlayer::playFile(char *p_name, int cycle, bool wait)
{
  if (gif_play.is_init == false)
  {
    return false;
  }

  gif_play.play_mode = PLAY_FILE;
  gif_play.is_stop = true;
  while(gif_play.is_busy)
  {
    delay(1);
  }
  gif_play.is_stop = false;


  gif_play.play_cycle = cycle;

  strcpy(gif_play.file_name, p_name);
  
  xSemaphoreGive(play_semaphore);

  if (wait == true)
  {
    gif_play.is_busy = true;
    while(gif_play.is_busy)
    {
      delay(1);
    }
  }
  else
  {
    gif_play.is_busy = true;
  }
  return true;
}

bool GifPlayer::playBuff(uint8_t *p_buff, uint32_t length, int cycle, bool wait)
{
  if (gif_play.is_init == false)
  {
    return false;
  }

  gif_play.play_mode = PLAY_BUFF;
  gif_play.is_stop = true;
  while(gif_play.is_busy)
  {
    delay(1);
  }
  gif_play.is_stop = false;


  gif_play.play_cycle = cycle;
  gif_play.play_buff = p_buff;
  gif_play.play_buff_length = length;

  xSemaphoreGive(play_semaphore);

  if (wait == true)
  {
    gif_play.is_busy = true;
    while(gif_play.is_busy)
    {
      delay(1);
    }
  }
  else
  {
    gif_play.is_busy = true;
  }
  return true;
}

void GifPlayer::playStop(void)
{
  gif_play.is_stop = true;
  while(isBusy())
  {
    delay(1);
  }

  gif_play.is_stop = false;
}

void GifPlayer::waitStop(void)
{
  while(gif_play.is_busy)
  {
    delay(1);
  }
}

bool GifPlayer::isBusy(void)
{
  return gif_play.is_busy | gif_play.is_draw;  
}

int GifPlayer::getPlayCycle(void)
{
  return decoder.getCycleNo();
}




void screenClearCallback(void) {
}

void updateScreenCallback(void) {
  #ifdef LCD_FRAMEBUFFER
  xSemaphoreGive(draw_semaphore);
  #else  
  GO.lcd.setWindow(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
  #endif
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  frame[y*LCD_WIDTH + x] = 0;
}

void drawLineCallback(int16_t x, int16_t y, uint8_t *buf, int16_t w, uint16_t *palette, int16_t skip) {

  #ifdef LCD_FRAMEBUFFER

  uint8_t pixel;
  uint32_t pos;
  
  for (int i = 0; i < w; i++) 
  {
    uint16_t color_h;
    uint16_t color_l;

    pos = y*LCD_WIDTH + (x+i);

    pixel = buf[i];
    if (pixel != skip)
    {            
      color_h = palette[pixel];
      color_l = (color_h>>8);
      color_h = (color_h<<8);
      frame[pos] = color_h | color_l;
    }     
  }   

  #else  

  uint8_t pixel;
  int16_t endx = x + w - 1;
  uint16_t buf565[w];
  
  for (int i = 0; i < w; ) 
  {
    int n = 0;
    while (i < w) 
    {
      pixel = buf[i++];
      if (pixel == skip) break;
      buf565[n++] = palette[pixel];
    }
    if (n) 
    {
      GO.lcd.setWindow(x + i - n, y, endx, y);
      GO.lcd.pushColors(buf565, n);
    }
  }   

  #endif
}




bool fileSeekCallback(unsigned long position) {
    gif_play.play_buff_seek = position;
    return true;
}

unsigned long filePositionCallback(void) {
    return gif_play.play_buff_seek;
}

int fileReadCallback(void) {
  return gif_play.play_buff[gif_play.play_buff_seek++];
}

int fileReadBlockCallback(void * buffer, int numberOfBytes) {
    memcpy(buffer, &gif_play.play_buff[gif_play.play_buff_seek], numberOfBytes);
    gif_play.play_buff_seek += numberOfBytes;
    return numberOfBytes;
}




bool gifplayStartFile(char *p_name)
{  
  bool started;
  uint8_t buf_i;

  File file = SPIFFS.open(p_name);
  if(!file)
  {
    Serial.println("Failed to open file for reading");
    return false;
  }

  gif_play.gif_length = file.size();

  if (gif_play.gif_buf == NULL)
  {
    gif_play.gif_buf = (uint8_t *)ps_malloc(gif_play.gif_length);
  }
  else
  {
    gif_play.gif_buf = (uint8_t *)ps_realloc((void *)gif_play.gif_buf, gif_play.gif_length);
  }

  if (gif_play.gif_buf == NULL)
  {
    Serial.print("Failed to malloc : ");
    Serial.println(gif_play.gif_length);
    file.close();
    return false;
  }  


  file.read((uint8_t *)gif_play.gif_buf, gif_play.gif_length);
  file.close();

  gif_play.play_buff = gif_play.gif_buf;
  gif_play.play_buff_seek = 0;
  decoder.startDecoding();

  started = false;  
  while(1)
  {
    decoder.decodeFrame();
    delay(1);

    if (gif_play.play_cycle > 0)
    {
      if (decoder.getCycleNo() >= (gif_play.play_cycle + 1))
      {
        break;
      }
    }

    if (gif_play.is_stop == true)
    {
      break;
    }
  }

  return true;
}

bool gifplayStartBuff(uint8_t *p_buff, uint32_t length_in)
{  
  while(1)
  {
    decoder.decodeFrame();
    delay(1);

    if (gif_play.play_cycle > 0)
    {
      if (decoder.getCycleNo() >= (gif_play.play_cycle + 1))
      {
        break;
      }
    }

    if (gif_play.is_stop == true)
    {
      break;
    }
  }

  return true;
}
