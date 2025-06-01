#ifndef MENU_H
#define MENU_H

#include <Adafruit_ILI9341.h>

// Declaraciones
void mostrarMenuSeleccion(Adafruit_ILI9341 &tft, int &modoActual, int pinUp, int pinDown, int pinFire, int pinBuzzer);
void dibujarFondoEspacio(Adafruit_ILI9341 &tft);
void animacionInicio(Adafruit_ILI9341 &tft, int pinFire, int pinBuzzer);
void animacionGameOver(Adafruit_ILI9341 &tft, int pinReset);

void playTone(int pinBuzzer, int frequency, int duration); // para sonidos simples

#endif
