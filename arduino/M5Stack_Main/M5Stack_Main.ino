#include <M5Stack.h>
#include "./src/driver/driver.h"



namespace AppPlayTTS      { extern void setup(); extern void loop(); }
namespace AppPlayRadio    { extern void setup(); extern void loop(); }
namespace AppWifiSetup    { extern void setup(); extern void loop(); }


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  menuInit();

  menuAdd("PlayTTS",      AppPlayTTS::setup,      AppPlayTTS::loop);  
  menuAdd("PlayRadio",    AppPlayRadio::setup,    AppPlayRadio::loop);  
  menuAdd("WiFi Setup",   AppWifiSetup::setup,    AppWifiSetup::loop);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  menuUpdate();
}



