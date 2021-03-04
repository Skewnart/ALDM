
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define IR_output 4
#define LED_PIN 3
#define LED_COUNT 16
#define BRIGHTNESS 10

#define CHANGEPERSEC 20
#define STAYALIVE 5

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int which;
int colorstep;
int r, g, b;
int stayalive;

int numbLed;

void setup() {
  Serial.begin(9600);
  pinMode(IR_output,INPUT);
  
  which = 0;
  colorstep = 0;
  r = 0;
  g = 255;
  b = 255;
  stayalive = 0;

  numbLed = 0;
  
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
}

int realLed(int value){
  return (2*(strip.numPixels()-1)-value)%16;
}

void loop() {
  while(true){
    if(digitalRead(IR_output)==HIGH){
      stayalive = STAYALIVE * CHANGEPERSEC;
    }

    if (which+1 <= numbLed)
      strip.setPixelColor(realLed(which), 255,255,255);
      
    which = (which+1)%strip.numPixels();

    switch(colorstep){
      case 0 : b -= 5; break;
      case 1 : r += 5; break;
      case 2 : g -= 5; break;
      case 3 : b += 5; break;
      case 4 : r -= 5; break;
      case 5 : g += 5; break;
    }
    if ((r+g+b)%255 == 0)
      colorstep = (colorstep+1)%6;

    if (stayalive > 0 && numbLed < 16){
      numbLed++;
    }
    else if (stayalive == 0 && numbLed > 0){
      numbLed--;
    }

    for(int i = 0;i < LED_COUNT; i++){
      if (i+1 <= numbLed){
        strip.setPixelColor(realLed((which+i)%strip.numPixels()), (int)(255-(255-r)*i/LED_COUNT),(int)(255-(255-g)*i/LED_COUNT),(int)(255-(255-b)*i/LED_COUNT));
      }
      else{
        strip.setPixelColor(realLed((which+i)%strip.numPixels()), 0, 0, 0);
      }
    }

    if (stayalive > 0) stayalive--;
    
    strip.show();
    delay(1000/CHANGEPERSEC);
  }
}
