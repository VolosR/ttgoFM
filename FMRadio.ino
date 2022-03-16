#include <Wire.h>
#include <TEA5767Radio.h>
#include <TFT_eSPI.h> 
#include "fm.h"
#include "fonts.h"

#define color1 0xC618
#define color2  0xE600
#define color3 0x2E35

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

int backlight[5] = {10,30,60,120,220};
byte br=1;

TFT_eSPI tft = TFT_eSPI(); 
TEA5767Radio radio = TEA5767Radio();

String stations[5]={"HomeFM", "Radio101","RnR Now", "Best FM","Classic "};
String freq[5]={"88.1","89.7","91.8","96.6","93.3"};

int sx=20;
int sy=130;
int fx=88;
int fy=134;

int chosen=0;

void setup() {

    pinMode(0,INPUT_PULLUP);
    pinMode(35,INPUT_PULLUP);

    Wire.begin(26,27);
    radio.setFrequency(freq[chosen].toFloat());
    
    tft.init();
    tft.setSwapBytes(true);
    tft.pushImage(0,0,135,240,fm); 
    
    
    ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
    ledcAttachPin(TFT_BL, pwmLedChannelTFT);
    ledcWrite(pwmLedChannelTFT, backlight[br]);

    for(int i=0;i<5;i++){
    tft.setTextColor(color3,TFT_BLACK);  
    tft.drawString(stations[i],sx,sy+(i*15),2);
    tft.setTextFont(0);
    tft.setTextColor(color2,TFT_BLACK);  
    tft.drawString(freq[i],fx,fy+(i*15));
    }

    tft.setTextColor(color1,TFT_BLACK); 
    tft.fillCircle(12,sy+8,3,TFT_WHITE);
    tft.setFreeFont(&DSEG7_Classic_Regular_36);
    tft.drawString(freq[chosen],14,84);
}

int b=0;
int press2=0;
void loop() {

  if(digitalRead(0)==0)
  {
    if(b==0)
    {
      b=1;
      tft.fillCircle(12,sy+8+(chosen*15),3,TFT_BLACK);
      chosen++;
     
      if(chosen>4)
       chosen=0;
       tft.fillCircle(12,sy+8+(chosen*15),3,TFT_WHITE);
       radio.setFrequency(freq[chosen].toFloat());
       tft.drawString(freq[chosen],14,84);
      }
  }else b=0;

   if(digitalRead(35)==0){
   if(press2==0)
     {press2=1;
      br++;
     if(br>=5)
     br=0;
   ledcWrite(pwmLedChannelTFT, backlight[br]);}
   }else press2=0;
}
