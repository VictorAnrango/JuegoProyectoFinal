#include "Bala.h"

Bala::Bala() : x(-10), y(-10), activa(false) {}

void Bala::disparar(int naveX, int naveY) {
    x = naveX;
    y = naveY - 10;
    activa = true;
}

void Bala::mover(Adafruit_ILI9341 &tft) {
    tft.fillRect(x - 1, y, 3, 6, ILI9341_BLACK);
    y -= 6;
    if (y > 0) {
        tft.fillRect(x - 1, y, 3, 6, ILI9341_WHITE);
    } else {
        activa = false;
    }
}

void Bala::borrar(Adafruit_ILI9341 &tft) {
    tft.fillRect(x - 1, y, 3, 6, ILI9341_BLACK);
}
