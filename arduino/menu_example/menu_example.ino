#include <M5Stack.h>
#include "src/driver/menu.h"


extern void imu_setup();
extern void imu_loop();
extern void tetris_setup(void);
extern void tetris_loop(void);
extern void sprite_setup();
extern void sprite_loop();
extern void sprite2_setup(void);
extern void sprite2_loop(void);
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  menuInit();

  menuAdd("IMU 시험", imu_setup, imu_loop);
  menuAdd("Tetris", tetris_setup, tetris_loop);
  menuAdd("Sprite1 Demo", sprite_setup, sprite_loop);
  menuAdd("Sprite2 Demo", sprite2_setup, sprite2_loop);
  menuAdd("테스트 1", NULL, NULL);
  menuAdd("테스트 2", NULL, NULL);
  menuAdd("테스트 3", NULL, NULL);
  menuAdd("테스트 4", NULL, NULL);
  menuAdd("테스트 5", NULL, NULL);
  menuAdd("테스트 6", NULL, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  menuUpdate();
}


