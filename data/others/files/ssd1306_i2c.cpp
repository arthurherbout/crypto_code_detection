/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch

Credits for parts of this code go to Mike Rankin. Thank you so much for sharing!
*/

#include "ssd1306_i2c.h"
#include <Wire.h>
#include "font.h"

SSD1306::SSD1306(int i2cAddress, int sda, int sdc)
{
  myI2cAddress = i2cAddress;
  mySda = sda;
  mySdc = sdc;  
}

void SSD1306::init() {
  Wire.begin(mySda, mySdc);
  Wire.setClock(400000); 
  sendInitCommands();
  reset_display();
}

void SSD1306::reset_display(void)
{
  displayOff();
  clear();
  display();
  displayOn();
}

void SSD1306::displayOn(void)
{
    sendCommand(0xaf);        //display on
}

void SSD1306::displayOff(void)
{
  sendCommand(0xae);		//display off
}

void SSD1306::clear(void) {
    memset(buffer, 0, (128*64 / 8));
}

void SSD1306::display(void) {

    for (uint16_t i=0; i<(128*64/8); i++) {
      // send a bunch of data in one xmission
      Wire.beginTransmission(myI2cAddress);
      Wire.write(0x40);
      for (uint8_t x=0; x<16; x++) {
        Wire.write(buffer[i]);
        i++;
      }
      i--;
      yield();
      Wire.endTransmission();
    }


}

void SSD1306::setPixel(int x, int y) {
  if (x >= 0 && x < 128 && y >= 0 && y < 64) {
     buffer[x+ (y/8)*128] |=  (1 << (y&7));
  }
}

void SSD1306::setChar(int x, int y, unsigned char data) {
  for (int i = 0; i < 8; i++) {
    if (bitRead(data, i)) {
     setPixel(x,y + i);
    }   
  }   
}

void SSD1306::drawString(int x, int y, String text) {
  for (int j=0; j < text.length(); j++) {
    for(int i=0;i<8;i++) {
      unsigned char charColumn = pgm_read_byte(myFont[text.charAt(j)-0x20]+i);
      for (int pixel = 0; pixel < 8; pixel++) {
        if (bitRead(charColumn, pixel)) {
          if (myIsFontScaling2x2) {
             setPixel(x + 2*(j* 8 + i),y + 2*pixel);
             setPixel(x + 2*(j* 8 + i)+1,y + 2*pixel +1);
             setPixel(x + 2*(j* 8 + i)+1,y + 2*pixel);
             setPixel(x + 2*(j* 8 + i),y + 2*pixel +1);
          } else {
             setPixel(x + j* 8 + i,y + pixel);  
          }
        }   
      }
    }
  }
}

void SSD1306::setFontScale2x2(bool isFontScaling2x2) {
  myIsFontScaling2x2 = isFontScaling2x2;
}

void SSD1306::drawBitmap(int x, int y, int width, int height, const char *bitmap) {
  for (int i = 0; i < width * height / 8; i++ ){
    unsigned char charColumn = 255 - pgm_read_byte(bitmap + i);
    for (int j = 0; j < 8; j++) { 
      int targetX = i % width + x;
      int targetY = (i / (width)) * 8 + j + y;
      if (bitRead(charColumn, j)) {
        setPixel(targetX, targetY);  
      }
    }
  }  
}

void SSD1306::drawXbm(int x, int y, int width, int height, const char *xbm) {
  if (width % 8 != 0) {
    width =  ((width / 8) + 1) * 8;
  }
  for (int i = 0; i < width * height / 8; i++ ){
    unsigned char charColumn = pgm_read_byte(xbm + i);
    for (int j = 0; j < 8; j++) { 
      int targetX = (i * 8 + j) % width + x;
      int targetY = (8 * i / (width)) + y;
      if (bitRead(charColumn, j)) {
        setPixel(targetX, targetY);  
      }
    }
  }    
}

void SSD1306::sendCommand(unsigned char com)
{
  Wire.beginTransmission(myI2cAddress);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}

void SSD1306::sendInitCommands(void)
{
  sendCommand(0xae);		//display off
  sendCommand(0xa6);            //Set Normal Display (default)
  // Adafruit Init sequence for 128x64 OLED module
  sendCommand(0xAE);             //DISPLAYOFF
  sendCommand(0xD5);            //SETDISPLAYCLOCKDIV
  sendCommand(0x80);            // the suggested ratio 0x80
  sendCommand(0xA8);            //SSD1306_SETMULTIPLEX
  sendCommand(0x3F);
  sendCommand(0xD3);            //SETDISPLAYOFFSET
  sendCommand(0x0);             //no offset
  sendCommand(0x40 | 0x0);      //SETSTARTLINE
  sendCommand(0x8D);            //CHARGEPUMP
  sendCommand(0x14);
  sendCommand(0x20);             //MEMORYMODE
  sendCommand(0x00);             //0x0 act like ks0108
  
  sendCommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
  //sendCommand(0xA0);
  
  sendCommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
  //sendCommand(0xC0);
  
  sendCommand(0xDA);            //0xDA
  sendCommand(0x12);           //COMSCANDEC
  sendCommand(0x81);           //SETCONTRAS
  sendCommand(0xCF);           //
  sendCommand(0xd9);          //SETPRECHARGE 
  sendCommand(0xF1); 
  sendCommand(0xDB);        //SETVCOMDETECT                
  sendCommand(0x40);
  sendCommand(0xA4);        //DISPLAYALLON_RESUME        
  sendCommand(0xA6);        //NORMALDISPLAY             

  sendCommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
  //  sendCommand(0xa0);		//seg re-map 0->127(default)
  //  sendCommand(0xa1);		//seg re-map 127->0
  //  sendCommand(0xc8);
  //  delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendCommand(0xa7);  //Set Inverse Display  
  // sendCommand(0xae);		//display off
  sendCommand(0x20);            //Set Memory Addressing Mode
  sendCommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  // sendCommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)  
  
}

void SSD1306::nextFrameTick() {
    myFrameTick++;
    if (myFrameTick==myFrameWaitTicks && myFrameState == 0 || myFrameTick==myFrameTransitionTicks && myFrameState == 1) {
      myFrameState = (myFrameState + 1) %  2;
      if (myFrameState==FRAME_STATE_FIX) {
        myCurrentFrame = (myCurrentFrame + 1) % myFrameCount; 
      } 
      myFrameTick = 0; 
    }
    drawIndicators(myFrameCount, myCurrentFrame);
    
    switch(myFrameState) {
    case 0:
      (*myFrameCallbacks[myCurrentFrame])(0, 0);
      break;
    case 1:
      (*myFrameCallbacks[myCurrentFrame])(-128 * myFrameTick / myFrameTransitionTicks, 0);
      (*myFrameCallbacks[(myCurrentFrame + 1) % myFrameCount])(-128 * myFrameTick / myFrameTransitionTicks + 128, 0);
      break;
    }
    
}
void SSD1306::drawIndicators(int frameCount, int activeFrame) {
  for (int i = 0; i < frameCount; i++) {
    const char *xbm;
    if (activeFrame == i) {
       xbm = active_bits;
    } else {
       xbm = inactive_bits;  
    }
    drawXbm(64 - (12 * frameCount / 2) + 12 * i,56, 8, 8, xbm);
  }  
}
void SSD1306::setFrameCallbacks(int frameCount, void (*frameCallbacks[])(int x, int y)) {
  myFrameCount = frameCount;
  myFrameCallbacks = frameCallbacks;
}

void SSD1306::setFrameWaitTicks(int frameWaitTicks) {
  myFrameWaitTicks = frameWaitTicks; 
}
void SSD1306::setFrameTransitionTicks(int frameTransitionTicks) {
  myFrameTransitionTicks = frameTransitionTicks; 
}
int SSD1306::getFrameState() {
  return myFrameState; 
}
