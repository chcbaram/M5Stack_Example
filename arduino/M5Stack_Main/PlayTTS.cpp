#include <M5Stack.h>
#include "./src/driver/driver.h"





namespace AppPlayTTS
{

int8_t volume = 90;

void playTTS(void)
{
  audio.playTTS("안녕하세요. 저의 이름은 엠파이브스택 파이어에요. 이것은 구글 TTS를 이용해서 음성을 출력해 본것이에요. 조금은 느리지만 동작은 됩니다. ", "ko", false);  
}

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
  M5.Lcd.fillScreen(BLACK);

  playTTS();
}

void loop() {
  // put your main code here, to run repeatedly
  uint32_t pre_time;


  pre_time = micros();  
  M5.Lcd.setCursor(0, 0); 
  M5.Lcd.println("TTS");
  M5.Lcd.println(millis());

  if (audio.isBusy())
  {
    M5.Lcd.println("Playing");
  }
  else
  {
    M5.Lcd.println("Stop");
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
      playTTS();
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