#include "Meteorito.h"

Meteorito::Meteorito() : x(0), y(0), activo(false) {}

void Meteorito::inicializar(int ancho) {
    x = random(10, ancho - 10);
    y = random(-120, -20);
    activo = true;
}

void Meteorito::dibujar(Adafruit_ILI9341 &tft, int color) {
    // Cuerpo base del meteorito
    tft.fillCircle(x, y, 6, color);

    // Fragmentos irregulares para dar efecto de roca
    tft.fillCircle(x - 4, y + 2, 2, color);
    tft.fillCircle(x + 3, y - 3, 2, color);
    tft.fillCircle(x + 1, y + 4, 1, color);

    // Cráteres estilo retro
    tft.fillCircle(x - 1, y - 1, 1, ILI9341_DARKGREY);
    tft.fillCircle(x + 2, y + 1, 1, ILI9341_DARKGREY);
    tft.fillCircle(x - 3, y + 2, 1, ILI9341_DARKGREY);

    // Borde oscuro para dar efecto de volumen
    tft.drawCircle(x, y, 6, ILI9341_BLACK);
}

void Meteorito::borrar(Adafruit_ILI9341 &tft) {
    // Borra completamente el área usada
    tft.fillRect(x - 8, y - 8, 17, 17, ILI9341_BLACK);
}
