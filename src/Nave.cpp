#include "Nave.h"

Nave::Nave(int x0, int y0) : x(x0), y(y0) {}

void Nave::dibujar(Adafruit_ILI9341 &tft) {
    // Cuerpo central (blanco)
    tft.fillTriangle(x, y, x - 4, y + 10, x + 4, y + 10, ILI9341_WHITE);
    tft.fillRect(x - 2, y + 10, 4, 6, ILI9341_WHITE);

    // Cabina (azul claro)
    tft.fillRect(x - 1, y + 4, 2, 4, ILI9341_CYAN);

    // Alas rojas
    tft.fillTriangle(x - 4, y + 10, x - 10, y + 16, x - 4, y + 16, ILI9341_RED);
    tft.fillTriangle(x + 4, y + 10, x + 10, y + 16, x + 4, y + 16, ILI9341_RED);

    // Alas inferiores
    tft.fillRect(x - 10, y + 16, 3, 4, ILI9341_RED);
    tft.fillRect(x + 7, y + 16, 3, 4, ILI9341_RED);
}

void Nave::borrar(Adafruit_ILI9341 &tft) {
    // Borra con un margen de seguridad de 1 px extra por lado
    tft.fillRect(x - 11, y - 1, 22, 22, ILI9341_BLACK);
}
