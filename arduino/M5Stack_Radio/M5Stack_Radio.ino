#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
#include "audio_player.h"


char ssid[] = "your ssid"; 
char pass[] = "your password"; 

int status = WL_IDLE_STATUS;
WiFiClient  client;

typedef struct
{
  char *host;
  char *path;
  int   port;
} station_tbl_t;

station_tbl_t station_tbl[] = 
{
  {"radiostreaming.ert.gr", "/ert-kosmos", 80},
  {"onair.radiogfm.kr", "/", 8003},
};


player_t player;


void connectStation(int station_no) 
{
  if (client.connect(station_tbl[station_no].host, station_tbl[station_no].port)) 
  {
    Serial.println("Connected now"); 
    M5.Lcd.println("Connected now");
  }
  client.print(String("GET ") + station_tbl[station_no].path + " HTTP/1.1\r\n" +
              "Host: " + station_tbl[station_no].host + "\r\n" + 
              "Connection: close\r\n\r\n");   
}

static renderer_config_t *create_renderer_config()
{
  renderer_config_t *renderer_config = (renderer_config_t *)calloc(1, sizeof(renderer_config_t));

  renderer_config->bit_depth = I2S_BITS_PER_SAMPLE_16BIT;
  renderer_config->i2s_num = I2S_NUM_0;
  renderer_config->sample_rate = 44100;
  renderer_config->sample_rate_modifier = 1.0;
  renderer_config->output_mode = DAC_BUILT_IN;
  return renderer_config;
}

void setup() {
  
  M5.begin();
  M5.Speaker.end();
  Serial.println("Start..");

  M5.Lcd.fillScreen(BLACK);  
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0, 0);


  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    M5.Lcd.print(".");    
  }
  M5.Lcd.println(" ");    
  




  Serial.println("WiFi connected");
  M5.Lcd.println("WiFi connected");
  


  connectStation(1);
  delay(2000);

  M5.Lcd.fillScreen(BLACK);

  if (!spiRamFifoInit()) 
  {
    printf("\n\nSPI RAM chip fail!\n");
    while(1);
  }
  renderer_init(create_renderer_config());

  player.command = CMD_NONE;
  player.decoder_status = UNINITIALIZED;
  player.decoder_command = CMD_NONE;
  player.buffer_pref = BUF_PREF_SAFE;
  player.media_stream = (media_stream_t *)calloc(1, sizeof(media_stream_t));  
  player.media_stream->content_type =  AUDIO_MPEG;
  audio_player_init(&player);
  audio_player_start();  
}

uint8_t mp3buff[4096];
uint32_t pre_time;
bool playing = false;

void loop() {
  int length;
  int buf_len;

	M5.BtnA.read();
	M5.BtnB.read();
	M5.BtnC.read();

  if (millis()-pre_time > 100)
  {
    pre_time = millis();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Web Radio");

    if (playing == true)
    {
      M5.Lcd.println("Playing");
    }
    else
    {
      M5.Lcd.println("Stoped ");
    }
  }

  if (M5.BtnB.wasReleased())
  {
    if (playing == false)
    {
      playing = true;
    }
    else
    {
      playing = false;
    }
  }


  length = client.available();
  buf_len = spiRamFifoFree();

  if (length > 2048)
  {
    length = 2048;
  }

  if (length > 0 && buf_len > 0)
  {
    
    if (length > buf_len)
    {
      length = buf_len;
    }
    uint32_t bytesread = client.read(mp3buff, length);

    if (playing == true)
    {
      audio_stream_consumer((const char *)mp3buff, bytesread, (void *)&player);
    }
  }  
}
