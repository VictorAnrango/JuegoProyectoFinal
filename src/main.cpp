#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Juego.h"

#define TFT_CS   6
#define TFT_DC   7
#define TFT_RST  10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

Juego juego(tft);

void setup() {
    juego.setup();
}

void loop() {
    juego.loop();
}