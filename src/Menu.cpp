#include "menu.h"

#include <Arduino.h> 

void mostrarMenuSeleccion(Adafruit_ILI9341 &tft, int &modoActual, int pinUp, int pinDown, int pinFire, int pinBuzzer) {
    int seleccion = 0;
    bool refrescarPantalla = true;
    unsigned long ultimaInteraccion = 0;
    const unsigned long tiempoRebote = 200;

    while (true) {
        unsigned long ahora = millis();

        if (refrescarPantalla) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setTextColor(ILI9341_WHITE);
            tft.setTextSize(2);
            tft.setCursor(20, 60);
            tft.print("SELECCIONA MODO");

            tft.setCursor(40, 120);
            tft.setTextColor(seleccion == 0 ? ILI9341_GREEN : ILI9341_WHITE);
            tft.print("> Esquivar");

            tft.setCursor(40, 150);
            tft.setTextColor(seleccion == 1 ? ILI9341_GREEN : ILI9341_WHITE);
            tft.print("> Destruir");

            refrescarPantalla = false;
        }

        if (digitalRead(pinUp) == HIGH && (ahora - ultimaInteraccion > tiempoRebote)) {
            seleccion = (seleccion == 0) ? 1 : 0;
            ultimaInteraccion = ahora;
            refrescarPantalla = true;
        }

        if (digitalRead(pinDown) == HIGH && (ahora - ultimaInteraccion > tiempoRebote)) {
            seleccion = (seleccion == 1) ? 0 : 1;
            ultimaInteraccion = ahora;
            refrescarPantalla = true;
        }

        if (digitalRead(pinFire) == HIGH && (ahora - ultimaInteraccion > tiempoRebote)) {
            modoActual = (seleccion == 0) ? 0 : 1;
            playTone(pinBuzzer, 523, 150); // SONIDO_SELECCION
            delay(200);
            break;
        }
    }
}

void dibujarFondoEspacio(Adafruit_ILI9341 &tft) {
    tft.fillScreen(ILI9341_BLACK);
    for (int i = 0; i < 50; i++) {
        int x = random(tft.width());
        int y = random(tft.height());
        uint16_t color = (random(2) == 0) ? ILI9341_WHITE : ILI9341_LIGHTGREY;
        tft.drawPixel(x, y, color);
    }

    tft.fillTriangle(60, 180, 70, 200, 50, 200, ILI9341_CYAN);
    tft.fillCircle(100, 40, 6, ILI9341_MAROON);
    tft.fillCircle(180, 80, 8, ILI9341_ORANGE);
    tft.fillCircle(140, 30, 4, ILI9341_DARKGREY);
}

void animacionInicio(Adafruit_ILI9341 &tft, int pinFire, int pinBuzzer) {
    dibujarFondoEspacio(tft);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 100);
    tft.print("NAVE VS METEORITOS");

    tft.setTextSize(1);
    tft.setCursor(40, 160);
    tft.print("Pulsa DISPARO para iniciar");

    while (digitalRead(pinFire) == LOW) {
        delay(10);
    }

    playTone(pinBuzzer, 440, 200); 
    playTone(pinBuzzer, 440, 300); 
    playTone(pinBuzzer, 440, 400);
    delay(500);

    for (int i = 0; i < tft.height(); i += 10) {
        tft.drawFastHLine(0, i, tft.width(), ILI9341_BLACK);
        delay(10);
    }
}

void animacionGameOver(Adafruit_ILI9341 &tft, int pinReset) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(ILI9341_RED);

    for (int i = 0; i < 3; i++) {
        tft.setCursor(40, 120);
        tft.print("GAME OVER"); 
        delay(300);
        tft.fillScreen(ILI9341_BLACK);
        delay(300);
    }

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(20, 200);
    tft.print("Presiona RESET");

    while (digitalRead(pinReset) == LOW) {
        delay(10);
    }
}

void playTone(int pinBuzzer, int frequency, int duration) {
    tone(pinBuzzer, frequency, duration);
    delay(duration + 20);
    noTone(pinBuzzer);
}
