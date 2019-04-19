#ifndef _WIFI_EDUBOT_H_
#define _WIFI_EDUBOT_H_

#include <Arduino.h>
#include <Preferences.h>


class WifiM5
{
  public:
    WifiM5();
    ~WifiM5();
    
    bool begin(uint32_t timeout = 3000);

    bool isBegin(void);
    bool isConnected(void);
    bool startBegin(uint32_t timeout);

  private:    
    bool is_init;    
    bool is_begin;
    bool is_connected;
    bool is_pref_load;

    String wifi_ssid;
    String wifi_password;   
    

    Preferences pref;

};


#endif