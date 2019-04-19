#include "wifi_m5.h"
#include <WiFi.h>




WifiM5::WifiM5()
{
  is_begin = false;
  is_connected = false;
  is_pref_load = false;
} 

WifiM5::~WifiM5()
{
    
}

bool WifiM5::begin(uint32_t timeout)
{
  bool ret = true;

  pref.begin("wifi-config");
  wifi_ssid = pref.getString("WIFI_SSID");
  wifi_password = pref.getString("WIFI_PASSWD");  

  if(wifi_ssid.length() <= 0) 
  {
    return false;
  } 

  is_pref_load = true;

  ret = startBegin(timeout);

  return ret;
}

bool WifiM5::isConnected(void)
{
  return is_connected;
}

bool WifiM5::isBegin(void)
{
  return is_begin;
}

bool WifiM5::startBegin(uint32_t timeout)
{
  bool ret = false;
  uint32_t pre_time;

  is_begin = false;
  is_connected = false;

  if (is_pref_load == true)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

    pre_time = millis();
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(100);

      if (millis()-pre_time >= timeout)
      {
        return false;
      }
    }
    is_begin = true;
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}
