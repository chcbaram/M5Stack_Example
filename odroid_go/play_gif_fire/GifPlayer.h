#ifndef _GIFPLAYER_H_
#define _GIFPLAYER_H_

#include <Arduino.h>
#include <odroid_go.h>
#include "SPIFFS.h"  





class GifPlayer
{
  public:
    GifPlayer();
    ~GifPlayer();
    
    bool begin(void);
    bool isInit(void);
    int  getPlayCycle(void);

    bool playFile(char *p_name, int cycle, bool wait);
    bool playBuff(uint8_t *p_buff, uint32_t length,int cycle, bool wait);
    void playStop(void);
    void waitStop(void);
    bool isBusy(void);


  private:    
};



#endif