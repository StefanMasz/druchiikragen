#include <RCSwitch.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

int normalOn = 1;

RCSwitch mySwitch = RCSwitch();

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:strip.setPixelColor(i,0);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(11, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
 

  strip.begin();
  strip.show();
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Empfänger ist an Interrupt-Pin "0" - Das ist am UNO der Pin2
}

void loop() {
  
  if (mySwitch.available()) // Wenn ein Code Empfangen wird...
  {
    
    Serial.println("empfaengt");  
    int value = mySwitch.getReceivedValue(); // Empfangene Daten werden unter der Variable "value" gespeichert.
  
    if (value == 0) // Wenn die Empfangenen Daten "0" sind, wird "Unbekannter Code" angezeigt.
    {
      Serial.println("Unbekannter Code");
    } 
    
    else // Wenn der Empfangene Code brauchbar ist, wird er hier an den Serial Monitor gesendet.
    {
      Serial.print("Empfangen: ");
      Serial.println( value );
      if (value == 9762)//Verarbeitung: Wenn der taster gedrückt ist (Das Spannungssignal ist hoch)
      { //Programmabschnitt des IF-Befehls öffnen.
        normalOn = 1;
      } //Programmabschnitt des IF-Befehls schließen.
      if (value == 1263){
        normalOn = 0;      
      }
    }

    mySwitch.resetAvailable(); // Hier wird der Empfänger "resettet"
  }

  if (normalOn == 1){
    colorWipe(strip.Color(255, 0, 0), 100); // Red
    colorWipe(strip.Color(0, 255, 0), 100); // Green
    colorWipe(strip.Color(0, 0, 255), 100); // Blue
  } 
  if (normalOn == 0){
    for(uint16_t i = 0; i < strip.numPixels(); i++) 
    {
        strip.setPixelColor(i,0);
        strip.setPixelColor(1,255);
        strip.show();
  }
  normalOn = -1;  
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
