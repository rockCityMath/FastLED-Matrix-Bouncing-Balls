#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 12;
const uint8_t kMatrixHeight = 13;

#define LED_PIN 2
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define LAST_VISIBLE_LED 155

CRGB leds[ NUM_LEDS ];

//Returns pixel's index based on X, Y representation passed in (Credit: Macetech's XY Map Generator)
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint8_t XYTable[] = {
   144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
   143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132,
   120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
   119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108,
    96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107,
    95,  94,  93,  92,  91,  90,  89,  88,  87,  86,  85,  84,
    72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,
    71,  70,  69,  68,  67,  66,  65,  64,  63,  62,  61,  60,
    48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    47,  46,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,
    24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,
    23,  22,  21,  20,  19,  18,  17,  16,  15,  14,  13,  12,
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}

//Interface for ball
class Ball {
  public:

    //Coordinates and velocity vectors
    uint8_t x;
    uint8_t y;
    uint8_t vx;
    uint8_t vy;

    Ball(uint8_t x, uint8_t y, uint8_t vx, uint8_t vy) {
      this->x = x;
      this->y = y;
      this->vx = vx;
      this->vy = vy;
    }

    //Per frame updates
    void updateBall() {
      x = x+vx;
      y = y+vy;
    
      if(x < 0 || x >= kMatrixWidth) {
        vx = -vx;
        x = x+vx;
      }
      if(y < 0 || y >= kMatrixHeight) {
        vy = -vy;
        y = y+vy;
      }
    }
};

//Declare balls (vx and vy having magnitude of 1 looks smoothest on smaller displays imo)
Ball cyanBall(1, 9, 1, 1);
Ball whiteBall(11, 5, -1, 1);
Ball purpleBall(6, 11, -1, -1);

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  Serial.begin(57600);
  delay(100);

}

void loop() {

  //Manages trail
  EVERY_N_MILLISECONDS(25) {
    fadeToBlackBy(leds, NUM_LEDS, 150);
  }

  //Update each ball
  cyanBall.updateBall();
  whiteBall.updateBall();
  purpleBall.updateBall();

  //Calculate the index of the ball based on its XY coordinates
  uint8_t cyanBallCoords = XY(cyanBall.x, cyanBall.y);
  uint8_t whiteBallCoords = XY(whiteBall.x, whiteBall.y);
  uint8_t purpleBallCoords = XY(purpleBall.x, purpleBall.y);

  //Set the appropriate LED to the appropriate ball color
  leds[cyanBallCoords] = CRGB::Cyan;
  leds[whiteBallCoords] = CRGB::White;
  leds[purpleBallCoords] = CRGB::Purple;
  
  FastLED.show();

  int potVal = analogRead(3);
  uint8_t delayAmount = map(potVal, 0, 1023, 0, 200);
  delay(delayAmount);         //Controls speed of balls (fun to control with a pot)

}
