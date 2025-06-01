#ifndef BALA_H
#define BALA_H

#include <Adafruit_ILI9341.h>

class Bala {
public:
    int x, y;
    bool activa;

    Bala();

    void disparar(int naveX, int naveY);
    void mover(Adafruit_ILI9341 &tft);
    void borrar(Adafruit_ILI9341 &tft);
};

#endif
