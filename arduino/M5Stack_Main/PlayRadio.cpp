#include <M5Stack.h>
#include "./src/driver/driver.h"





namespace AppPlayRadio
{


int8_t volume = 90;

void setup() {
  // put your setup code here, to run once:
  bool ret;
  
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(3);

  audio.begin();

  ret = wifi.begin();

  if (ret == true)
  {
    M5.Lcd.println("WiFi OK");  
    delay(2000);
  }
  else
  {
    M5.Lcd.println("WiFi Fail");  
    delay(2000);
  }

  audio.setVolume(volume);
  audio.playURL("onair.radiogfm.kr:8003");

  M5.Lcd.fillScreen(BLACK);
}

void loop() {
  // put your main code here, to run repeatedly
  uint32_t pre_time;


  pre_time = micros();
  M5.Lcd.setCursor(0, 0); 
  M5.Lcd.println("Radio");
  M5.Lcd.println(millis());

  if (audio.isBusy())
  {
    M5.Lcd.println("Playing");
  }
  else
  {
    M5.Lcd.println("Stop    ");
  }
  M5.Lcd.print("Volume : ");
  M5.Lcd.print(volume);
  M5.Lcd.println("   ");
  
  delay(50);

  if (M5.BtnB.read())
  {
    while(M5.BtnB.read());

    if (audio.isBusy())
    {
      audio.playStop();
    }
    else
    {
      audio.playURL("onair.radiogfm.kr:8003");
    }
  }  

  if (M5.BtnA.read())
  {
    while(M5.BtnA.read());
    if (volume < 100)
    {
      volume += 10;
      volume = constrain(volume, 0, 100);
      audio.setVolume(volume);
    }
  }

  if (M5.BtnC.read())
  {
    while(M5.BtnC.read());
    if (volume > 0)
    {
      volume -= 10;
      volume = constrain(volume, 0, 100);
      audio.setVolume(volume);
    }
  }

}

}