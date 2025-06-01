#ifndef METEORITO_H
#define METEORITO_H

#include <Adafruit_ILI9341.h>

class Meteorito {
public:
    int x, y;
    bool activo;

    Meteorito();

    void inicializar(int ancho);
    void dibujar(Adafruit_ILI9341 &tft, int color);
    void borrar(Adafruit_ILI9341 &tft);
};

#endif
