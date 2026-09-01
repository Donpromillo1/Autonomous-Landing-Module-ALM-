#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//#define LED_PIN   16
#define PIN       16 // pin am ESP32 KS5016 board
// How many NeoPixels are attached to the Arduino?
//#define LED_COUNT  7
//#define NUMPIXELS 7
int NUMPIXELS = 7; // Anzahl der LEDs
// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

//  Anlegen des Neopixel streifen objekts:
// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Erklärung zum angelegten Objekt:
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //set the resolution to 16 bits (0-4095)
  analogReadResolution(16);
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  // LED streifen objekt aktivieren
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  /*strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);*/
}

void loop() {
  //pixels.clear(); // Set all pixel colors to 'off'
  // read the analog / millivolts value for pin 0:
  int analogValue = analogRead(32);
  int analogVolts = analogReadMilliVolts(32);
  // gemessene Werte Batvoll = 12,6 V und Batfastleer = 10,5 V
  int Batvoll = 50000;
  int Batfastleer = 41400;
  
  // bool für Batstate anlegen falls library aus code ensteht
  // print out the values you read:
  Serial.printf("ADC analog value = %d\n", analogValue);
  Serial.printf("ADC millivolts value = %d\n", analogVolts);

  delay(100);  // delay in between reads for clear read from serial

  // Rote LEDs am Streifen sollen leuchten wenn batterie fast leer ist
  if ( analogValue <= Batfastleer )
  {
    int i = 7;
    Serial.println("Bat volt below 10.5");
    //colorWipe(strip.Color(255,   0,   0)     , 50); // rgb wert auf rot
    //strip.show();
    pixels.clear();
    delay (100);
    pixels.setPixelColor(i, pixels.Color(50, 0, 0));

    pixels.show();
    // hier code zum Stop kommando
    delay(100);
  }
  if(analogValue >= Batvoll)
  {
    Serial.println("Bat volt above 12.5");
    pixels.clear();
    delay (100);
    pixels.setPixelColor(NUMPIXELS, pixels.Color(0, 50, 0));

    pixels.show();
  }
}
/*
// schleife für methode colorWipe 
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
*/
