#include <odroid_go.h>
#include "GifPlayer.h"


GifPlayer gif_player;


void setup() {
  // put your setup code here, to run once:
  GO.begin();
  GO.Speaker.write(0);

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

  if (GO.JOY_X.wasAxisPressed() == 2) 
  {
    index = (index - 1) % gif_count;
    gif_player.playStop();
    GO.lcd.fillRoundRect((320-100)/2, (240-100)/2, 100, 100, 5, YELLOW);
    GO.lcd.setTextDatum(MC_DATUM);
    GO.lcd.setTextColor(TFT_BLUE, TFT_YELLOW);
    GO.lcd.drawString(String(index),160,120,8);    
    GO.lcd.drawString("Loading",160,200,4);    
  }
  if (GO.BtnA.wasReleased()) 
  {
    gif_player.playStop();
    GO.lcd.fillRoundRect((320-100)/2, (240-100)/2, 100, 100, 5, YELLOW);
    GO.lcd.setTextDatum(MC_DATUM);
    GO.lcd.setTextColor(TFT_BLUE, TFT_YELLOW);
    GO.lcd.drawString(String(index),160,120,8);
    GO.lcd.drawString("Loading",160,200,4);    
  }
  if (GO.JOY_X.wasAxisPressed() == 1) 
  {
    index = (index + 1) % gif_count;
    gif_player.playStop();
    GO.lcd.fillRoundRect((320-100)/2, (240-100)/2, 100, 100, 5, YELLOW);
    GO.lcd.setTextDatum(MC_DATUM);
    GO.lcd.setTextColor(TFT_BLUE, TFT_YELLOW);
    GO.lcd.drawString(String(index),160,120,8);
    GO.lcd.drawString("Loading",160,200,4);        
  }
  GO.update();
}



