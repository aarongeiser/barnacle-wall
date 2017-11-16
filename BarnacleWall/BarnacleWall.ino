#include <FastLED.h>
#define NUM_LEDS 398
#define NUM_BARNACLES 43
#define NUM_SENSORS 7
#define DATA_PIN 0
#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

class Barnacle {
  public:
    int type;             // sm = 1, md = 2, lg = 3
    int center;           // center LED
    CHSV primaryColor;    // fiber optic color
    CHSV secondaryColor;  // body color

    // Constructor
    Barnacle(int _type, int _center, CHSV _primaryColor, CHSV _secondaryColor)
    {
      type = _type;
      center = _center;
      primaryColor = _primaryColor;
      secondaryColor = _secondaryColor;
    }

    /**
     * Update Barnacle
     */
    void setCluster(CHSV primary, CHSV secondary)
    {
      int center = this->center;
      this->primaryColor = primary;
      this->secondaryColor = secondary;
      
      if (this->type == 2) {
        leds[center] = primary;
        leds[(center + 2)] = secondary;
        leds[(center + 4)] = secondary;
        leds[(center + 6)] = secondary;
        leds[(center + 8)] = secondary;
        leds[(center + 10)] = secondary;
        leds[(center + 12)] = secondary;
      } else {
        leds[center] = primary;
        leds[(center + 1)] = secondary;
        leds[(center + 2)] = secondary;
        leds[(center + 3)] = secondary;
        leds[(center + 4)] = secondary;
        leds[(center + 5)] = secondary;
        leds[(center + 6)] = secondary;
      }
    }
};

class Sensor {
  public:
    int pin;      // Pin number
    int state;    // 0 = off, 1 = on
    int pressed;  // Is pressed?

    // Constructor
    Sensor(int _pin, int _state, _pressed)
    {
      pin = _pin;
      state = _state;
      pressed = _pressed;
    }
};

// Barnacle array
Barnacle *barnacles[44];

// Sensor array
Sensor *sensors[7];

// Section array
int sections[7][2] = {
  {0, 6},   // 0
  {6, 11},  // 1
  {10, 16}, // 2
  {16, 23}, // 3
  {21, 28}, // 4
  {27, 34}, // 5
  {35, 43}  // 6
};

// rotating "base color" used by many of the patterns
uint8_t gHue = 0; 

void setup() {
  Serial.begin(9600);

  // Sensor mapping
  sensors[0] = new Sensor(4, 0);
  sensors[1] = new Sensor(5, 0);
  sensors[2] = new Sensor(6, 0);
  sensors[3] = new Sensor(7, 0);
  sensors[4] = new Sensor(8, 0);
  sensors[5] = new Sensor(9, 0);
  sensors[6] = new Sensor(10, 0);

  // Enable pins
  for(int i = 0; i < NUM_SENSORS; ++i) {
    pinMode(sensors[i]->pin, INPUT_PULLUP);
  } 

  // Barnacle mapping
  barnacles[0] =  new Barnacle(1, 0, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[1] =  new Barnacle(1, 10, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[2] =  new Barnacle(1, 20, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[3] =  new Barnacle(1, 27, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[4] =  new Barnacle(3, 34, CHSV(64, 255, 255), CHSV(64, 255, 255));  // Large
  barnacles[5] =  new Barnacle(1, 41, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[6] =  new Barnacle(3, 50, CHSV(64, 255, 255), CHSV(64, 255, 255));  // Large
  barnacles[7] =  new Barnacle(1, 57, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[8] =  new Barnacle(2, 65, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[9] =  new Barnacle(1, 81, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[10] = new Barnacle(2, 92, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[11] = new Barnacle(3, 105, CHSV(64, 255, 255), CHSV(64, 255, 255)); // Large
  barnacles[12] = new Barnacle(2, 112, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[13] = new Barnacle(2, 125, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[14] = new Barnacle(1, 138, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[15] = new Barnacle(1, 145, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[16] = new Barnacle(1, 152, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[17] = new Barnacle(3, 159, CHSV(64, 255, 255), CHSV(64, 255, 255)); // Large
  barnacles[18] = new Barnacle(2, 166, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[19] = new Barnacle(1, 180, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[20] = new Barnacle(1, 190, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[21] = new Barnacle(2, 197, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[22] = new Barnacle(2, 210, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[23] = new Barnacle(1, 223, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[24] = new Barnacle(1, 232, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[25] = new Barnacle(3, 239, CHSV(64, 255, 255), CHSV(64, 255, 255)); // Large
  barnacles[26] = new Barnacle(2, 246, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[27] = new Barnacle(1, 253, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[28] = new Barnacle(3, 260, CHSV(64, 255, 255), CHSV(64, 255, 255)); // Large
  barnacles[29] = new Barnacle(1, 267, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[30] = new Barnacle(2, 274, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[31] = new Barnacle(1, 287, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[32] = new Barnacle(1, 295, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[33] = new Barnacle(1, 305, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[34] = new Barnacle(2, 312, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[35] = new Barnacle(3, 325, CHSV(64, 255, 255), CHSV(64, 255, 255)); // Large
  barnacles[36] = new Barnacle(1, 332, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[37] = new Barnacle(2, 339, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[38] = new Barnacle(2, 352, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[39] = new Barnacle(1, 365, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[40] = new Barnacle(1, 372, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[41] = new Barnacle(1, 379, CHSV(64, 255, 255), CHSV(64, 255, 255));
  barnacles[42] = new Barnacle(1, 386, CHSV(64, 255, 255), CHSV(64, 255, 255));

  // Initialize LEDs
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

/*
 * Main loop
 */
void loop() {
  checkInputs();
  ambient();
  renderEffects();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

/**
 * Check our inputs and set the button state
 */
void checkInputs() 
{
  // Read pins
  for(int i = 0; i < NUM_SENSORS; ++i) {
    if (digitalRead(sensors[i]->pin) == HIGH) {
      sensors[i]->state = 1;
    } else {
      sensors[i]->state = 0;
    } 
  } 
}

/**
 * Handle the button states to render effects
 */
void renderEffects() {
  // Render effects based on sensors touched
  for(int i = 0; i < NUM_SENSORS; ++i) {
    if (digitalRead(sensors[i]->state) == 1) {
      illuminate(sections[i]);
    }
  } 
}

void ambient()
{
  fadeToBlackBy(leds, NUM_LEDS, 6);
  int pos = random8(NUM_BARNACLES);
  barnacles[pos]->setCluster(CHSV(0, 255, 255), CHSV(random8(130, 165), 255, 255));
}

void illuminate(int section[])
{
  fadeToBlackBy(leds, NUM_LEDS, 3);
  int pos = random8(section[0], section[1]);
  barnacles[pos]->setCluster(CHSV(0, 255, 255), CHSV(random8(36, 64), 255, 255));
}

void lightning(int start) 
{
  int up = start;
  int down = start;
  
  for(int i = 0; i < NUM_BARNACLES; i++) {
    up = up + 1;
    down = down - 1;
    barnacles[up]->setCluster(CHSV(0, 255, 255), CHSV(45, 255, 255));
    barnacles[down]->setCluster(CHSV(0, 255, 255), CHSV(45, 255, 255));
  
  }
}
