#ifndef JUEGO_H
#define JUEGO_H

#include <Adafruit_ILI9341.h>
#include "Nave.h"
#include "Meteorito.h"
#include "Bala.h"
#include "Fondo.h"
#include "Menu.h"


#define NUM_METEORITOS 3
#define MAX_BALAS 100

class Juego {
public:
    Adafruit_ILI9341 &tft;
    Nave nave;
    Meteorito meteoritos[NUM_METEORITOS];
    Bala balas[MAX_BALAS];
    Fondo fondo;
    int score, vidas, nivel, meteorSpeed, meteorVelocidadBase;
    enum ModoJuego {ESQUIVAR = 0, DESTRUIR = 1};
    ModoJuego modoActual;
    int naveStep;
    bool botonAnterior;
    // Pines
    int pinRight, pinLeft, pinUp, pinDown, pinFire, pinReset, pinBuzzer;

    Juego(Adafruit_ILI9341 &pantalla)
        : tft(pantalla), nave(120, 280), score(0), vidas(3), nivel(1),
        meteorVelocidadBase(3), meteorSpeed(3), modoActual(ESQUIVAR), naveStep(5), botonAnterior(LOW),
        pinRight(21), pinLeft(18), pinUp(19), pinDown(20), pinFire(22), pinReset(23), pinBuzzer(8) {}

    void setup() {
        tft.begin();
        tft.setRotation(0);
        tft.fillScreen(ILI9341_BLACK);

        pinMode(pinRight, INPUT);
        pinMode(pinLeft, INPUT);
        pinMode(pinUp, INPUT);
        pinMode(pinDown, INPUT);
        pinMode(pinFire, INPUT);
        pinMode(pinReset, INPUT);
        pinMode(pinBuzzer, OUTPUT);

        
        randomSeed(analogRead(0));
        animacionInicio(tft, pinFire, pinBuzzer);
        int modoSeleccionado = static_cast<int>(modoActual);
        mostrarMenuSeleccion(tft, modoSeleccionado, pinUp, pinDown, pinFire, pinBuzzer);
        modoActual = static_cast<ModoJuego>(modoSeleccionado);

        iniciar();
    }

    void loop() {
        if (digitalRead(pinReset) == HIGH) {
            int modoSeleccionado = static_cast<int>(modoActual);
            mostrarMenuSeleccion(tft, modoSeleccionado, pinUp, pinDown, pinFire, pinBuzzer);
            modoActual = static_cast<ModoJuego>(modoSeleccionado);
        iniciar();
    }


        // Movimiento nave
        bool movido = false;
        if (digitalRead(pinLeft) == HIGH && nave.x > 10) { nave.borrar(tft); nave.x -= naveStep; movido = true; }
        if (digitalRead(pinRight) == HIGH && nave.x < tft.width() - 10) { nave.borrar(tft); nave.x += naveStep; movido = true; }
        if (digitalRead(pinUp) == HIGH && nave.y > 10) { nave.borrar(tft); nave.y -= naveStep; movido = true; }
        if (digitalRead(pinDown) == HIGH && nave.y < tft.height() - 15) { nave.borrar(tft); nave.y += naveStep; movido = true; }
        if (movido) nave.dibujar(tft);

        // Detección flanco de subida para disparo
        bool botonActual = digitalRead(pinFire);
        if (botonActual == HIGH && botonAnterior == LOW) {
            disparo();
        }
        botonAnterior = botonActual;

        // Mover todas las balas activas
        for (int i = 0; i < MAX_BALAS; i++) {
            if (balas[i].activa) {
                balas[i].mover(tft);
            }
        }
        
        fondo.actualizarYdibujar(tft, tft.height());
        moverMeteoritos();

        delay(30);
    }

    void iniciar() {
        tft.fillScreen(ILI9341_BLACK);
        fondo.iniciar(tft.width(), tft.height());
        nave.x = 120; nave.y = 280;
        score = 0; vidas = 3; nivel = 1;
        meteorSpeed = meteorVelocidadBase;
        for (int i = 0; i < NUM_METEORITOS; i++) meteoritos[i].inicializar(tft.width());
        for (int i = 0; i < MAX_BALAS; i++) balas[i].activa = false;
        nave.dibujar(tft);
        actualizarPuntaje();
        actualizarVidas();
        actualizarNivel();
    }

    void disparo() {
        for (int i = 0; i < MAX_BALAS; i++) {
            if (!balas[i].activa) {
                balas[i].disparar(nave.x, nave.y);
                break;
            }
        }
        playTone(880, 80);  
    }

    void moverMeteoritos() {
        for (int i = 0; i < NUM_METEORITOS; i++) {
            if (!meteoritos[i].activo) continue;

            meteoritos[i].borrar(tft);
            meteoritos[i].y += meteorSpeed;
            meteoritos[i].dibujar(tft, ILI9341_RED);

            // Colisión con nave
            if (meteoritos[i].y + 6 >= nave.y &&
                meteoritos[i].x >= nave.x - 10 && meteoritos[i].x <= nave.x + 10) {
                vidas--;
                actualizarVidas();
                meteoritos[i].y = 0;
                meteoritos[i].x = random(10, tft.width() - 10);
                if (vidas <= 0) gameOver();
            }

            // Salida de pantalla sin ser destruido
            if (meteoritos[i].y > tft.height()) {
                if (modoActual == DESTRUIR) {
                    vidas--;
                    actualizarVidas();
                    playTone(350, 200); 
                    delay(50);
                    if (vidas <= 0) gameOver();
                } else {
                    score++;
                    actualizarPuntaje();
                    actualizarNivel();
                }
                meteoritos[i].y = 0;
                meteoritos[i].x = random(10, tft.width() - 10);
            }

            for (int j = 0; j < MAX_BALAS; j++) {
                if (balas[j].activa &&
                    balas[j].y < meteoritos[i].y + 6 && balas[j].y > meteoritos[i].y - 6 &&
                    balas[j].x > meteoritos[i].x - 6 && balas[j].x < meteoritos[i].x + 6) {
                    meteoritos[i].borrar(tft);
                    balas[j].borrar(tft);
                    balas[j].activa = false;
                    playTone(220, 200); 
                    score++;
                    actualizarPuntaje();
                    actualizarNivel();
                    meteoritos[i].y = 0;
                    meteoritos[i].x = random(10, tft.width() - 10);
                    break;
                }
            }
        }
    }

    void actualizarPuntaje() {
        tft.fillRect(0, 0, 100, 20, ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_GREEN);
        tft.setTextSize(2);
        tft.print("Score: ");
        tft.print(score);
    }

    void actualizarVidas() {
        tft.fillRect(110, 0, 100, 20, ILI9341_BLACK);
        tft.setCursor(110, 0);
        tft.setTextColor(ILI9341_YELLOW);
        tft.setTextSize(2);
        tft.print("Vidas: ");
        tft.print(vidas);
        playTone(440, 200);  
    }

    void actualizarNivel() {
        int nuevoNivel = 1 + score / 5;
        if (nuevoNivel != nivel) {
            nivel = nuevoNivel;
            meteorSpeed = meteorVelocidadBase + (nivel - 1);
            tft.fillRect(220, 0, 100, 20, ILI9341_BLACK);
            tft.setCursor(220, 0);
            tft.setTextColor(ILI9341_CYAN);
            tft.setTextSize(2);
            tft.print("Lv: ");
            tft.print(nivel);
            playTone(440, 200); 
        }
    }

    void gameOver() {
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.setCursor(40, 130);
        tft.print("Puntaje final:");

        tft.setCursor(100, 160);
        tft.setTextSize(3);
        tft.setTextColor(ILI9341_GREEN);
        tft.print(score);

        delay(2000);  
        
        playTone(440, 500); 
        animacionGameOver(tft, pinReset);
        int modoSeleccionado = static_cast<int>(modoActual);
        mostrarMenuSeleccion(tft, modoSeleccionado, pinUp, pinDown, pinFire, pinBuzzer);
        modoActual = static_cast<ModoJuego>(modoSeleccionado);
        iniciar();

    }

    void playTone(int frequency, int duration) {
        tone(pinBuzzer, frequency, duration);
        delay(duration + 20);
        noTone(pinBuzzer);
    }
};

#endif