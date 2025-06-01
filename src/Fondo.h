#ifndef FONDO_H
#define FONDO_H

#include <Adafruit_ILI9341.h>

struct Estrella {
    int x, y;
    uint16_t color;
};

class Fondo {
private:
    static const int NUM_ESTRELLAS = 40;
    Estrella estrellas[NUM_ESTRELLAS];

public:
    void iniciar(int ancho, int alto) {
        uint16_t colores[] = {ILI9341_WHITE, ILI9341_YELLOW, ILI9341_CYAN, ILI9341_RED};
        for (int i = 0; i < NUM_ESTRELLAS; i++) {
            estrellas[i].x = random(0, ancho);
            estrellas[i].y = random(0, alto);
            estrellas[i].color = colores[random(0, 4)];
        }
    }

    void actualizarYdibujar(Adafruit_ILI9341 &tft, int alto) {
        for (int i = 0; i < NUM_ESTRELLAS; i++) {
            // Borrar estrella anterior
            tft.drawPixel(estrellas[i].x, estrellas[i].y, ILI9341_BLACK);

            // Mover estrella hacia abajo
            estrellas[i].y += 1;

            // Si sale de pantalla, volver a arriba
            if (estrellas[i].y >= alto) {
                estrellas[i].y = 0;
                estrellas[i].x = random(0, tft.width());
            }

            // Dibujar nueva posición
            tft.drawPixel(estrellas[i].x, estrellas[i].y, estrellas[i].color);
        }
    }
};

#endif
