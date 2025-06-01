#ifndef NAVE_H
#define NAVE_H

#include <Adafruit_ILI9341.h>

class Nave {
public:
    int x, y;

    Nave(int x0 = 120, int y0 = 280);

    void dibujar(Adafruit_ILI9341 &tft);
    void borrar(Adafruit_ILI9341 &tft);
};

#endif
