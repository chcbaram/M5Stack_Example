#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "def.h"


#include "./audio/audio.h"
#include "./hangul/PHan_Lib.h"
#include "./menu/menu.h"
#include "./wifi/wifi_m5.h"


extern WifiM5 wifi;
extern Audio audio;

void driverInit(void);


#endif