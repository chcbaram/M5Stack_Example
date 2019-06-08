#include <M5Stack.h>
#include "GifPlayer.h"


GifPlayer gif_player;


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  gif_player.begin();
}  


void loop() {  
  static uint32_t index = 0;
  static uint32_t gif_count = 5;


  if (gif_player.isBusy() != true)
  {    
    if (index == 0) gif_player.playFile("/badger.gif", 1, false);
    if (index == 1) gif_player.playFile("/blinka-wipe.gif", 1, false);
    if (index == 2) gif_player.playFile("/cpython-logo.gif", 1, false);
    if (index == 3) gif_player.playFile("/mu-editor.gif", 1, false);
    if (index == 4) gif_player.playFile("/pyportal-blinka.gif", 1, false);    
  }

  if (M5.BtnA.wasReleased()) 
  {
    index = (index - 1) % gif_count;
    gif_player.playStop();
    M5.Lcd.fillRoundRect((320-100)/2, (240-100)/2, 100, 100, 5, YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_YELLOW);
    M5.Lcd.drawString(String(index),160,120,8);    
    M5.Lcd.drawString("Loading",160,200,4);    
  }
  if (M5.BtnB.wasReleased()) 
  {
    gif_player.playStop();
    M5.Lcd.fillRoundRect((320-100)/2, (240-100)/2, 100, 100, 5, YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_YELLOW);
    M5.Lcd.drawString(String(index),160,120,8);
    M5.Lcd.drawString("Loading",160,200,4);    
  }
  if (M5.BtnC.wasReleased()) 
  {
    index = (index + 1) % gif_count;
    gif_player.playStop();
    M5.Lcd.fillRoundRect((320-100)/2, (240-100)/2, 100, 100, 5, YELLOW);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_YELLOW);
    M5.Lcd.drawString(String(index),160,120,8);
    M5.Lcd.drawString("Loading",160,200,4);        
  }
  M5.update();
}



