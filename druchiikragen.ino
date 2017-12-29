#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DATAPIN 7

#define PULSEPIN 3

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(74, DATAPIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int event_activatePulse;
int event_setdefault;
int event_switchoff;
int pulseRunning;
int flickerRunning;

void setup() {
  Serial.begin(9600);
  
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  
  pinMode(PULSEPIN, INPUT_PULLUP);

  event_setdefault = 1;
  event_activatePulse = 0; 
  event_switchoff = 0;
  pulseRunning = 0;
  flickerRunning = 0;
   
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  //oneColor(strip.Color(71, 5, 95));
  int pulsepushed = digitalRead(PULSEPIN); // Read the state of the switch
  if (pulsepushed == LOW && pulseRunning == 0){
    event_activatePulse = 1;
    delay(300);
  } else if(pulsepushed == LOW && pulseRunning == 1){
    event_switchoff = 1;
    delay(2000);
  }

  if (event_switchoff == 1){
    event_switchoff = 0;
    oneColor(strip.Color(0, 0, 0)); //out
    event_setdefault = 0;
    pulseRunning = 0;  
    flickerRunning = 0;  
  } else {
    if (event_activatePulse == 1 || pulseRunning == 1){
      pulse();
      pulseRunning = 1;
      flickerRunning = 0;
      event_activatePulse = 0;
      
      Serial.println("make it pulse!"); 
    } 
    
    if (event_setdefault == 1 || flickerRunning == 1) { 
      flicker();
      flickerRunning = 1;
      event_setdefault = 0;
      Serial.println("make it flicker!");
    }  
  }
  
}

void flicker(){
  int r = 125;
  int b = 75;
  
  for (uint16_t i=0; i < strip.numPixels(); i = i +2) {
      strip.setPixelColor(i, strip.Color(71, 5, 95));  
      strip.show();
  }
  
  for (int i = 0; i < 35; i++){
    int flickerLed = random(0,strip.numPixels());
    if (flickerLed % 2 == 0){
     strip.setPixelColor(flickerLed, strip.Color(50, 0, 75));
     strip.show();
    } 
  }
  
  delay(200); 
  
}

void oneColor (uint32_t color){
  for (uint16_t i=0; i < strip.numPixels(); i = i +2) {
      strip.setPixelColor(i, color);
      strip.show(); 
  }
}

void pulse (){
  //RGB 71, 5, 95
  float r = 71;
  float b = 95;

  while(r >= 25){
    for (uint16_t i=0; i < strip.numPixels(); i = i +2) {
      strip.setPixelColor(i, strip.Color(r, 0, b));  
      strip.show();
    }
    delay(200);
    r = r - (r / 1.1);
    b = b - (b / 1.1);
  } 

  while(r <= 71){
    for (uint16_t y=0; y < strip.numPixels(); y = y +2) {
      strip.setPixelColor(y, strip.Color(r, 0, b));  
      strip.show();
    }
    delay(200);
    r = r + (r * 1.1);
    b = b + (b * 1.1);
  }
}
