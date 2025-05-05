## Sebastian Pérez Capitano

# Código Práctica 7.

## Ejercicio 1.

```c
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"


AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;


void setup(){
  Serial.begin(115200);


  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();
  out -> SetGain(0.5);
  out -> SetPinout(42,7,5);
  aac->begin(in, out);
}


void loop(){
  if (aac->isRunning()) {
    aac->loop();
  } else {
    aac -> stop();
    Serial.printf("Sound Generator\n");
    delay(1000);
  }
}
```

## Ejercicio 2.

```c
#include "Audio.h"
#include "SD.h"
#include "FS.h"
// Digital I/O used
#define SD_CS 15
#define SPI_MOSI 8
#define SPI_MISO 17
#define SPI_SCK 18
#define I2S_DOUT 5
#define I2S_BCLK 42
#define I2S_LRC 7
Audio audio;
void setup(){
pinMode(SD_CS, OUTPUT);
digitalWrite(SD_CS, HIGH);
SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
Serial.begin(115200);
SD.begin(SD_CS);
audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
audio.setVolume(10); // 0...21
audio.connecttoFS(SD, "Ensoniq-ZR-76-01-Dope-77.wav");
}


void loop(){
audio.loop();
}
// optional


void audio_info(const char *info){
Serial.print("info "); Serial.println(info);
}
void audio_id3data(const char *info){ //id3 metadata
Serial.print("id3data ");Serial.println(info);
}
void audio_eof_mp3(const char *info){ //end of file
Serial.print("eof_mp3 ");Serial.println(info);
}
void audio_showstation(const char *info){
Serial.print("station ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
Serial.print("streaminfo ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
Serial.print("bitrate ");Serial.println(info);
}
void audio_commercial(const char *info){ //duration in sec
Serial.print("commercial ");Serial.println(info);
}
void audio_icyurl(const char *info){ //homepage
Serial.print("icyurl ");Serial.println(info);
}
void audio_lasthost(const char *info){ //stream URL played
Serial.print("lasthost ");Serial.println(info);
}
void audio_eof_speech(const char *info){
Serial.print("eof_speech ");Serial.println(info);
}


```
