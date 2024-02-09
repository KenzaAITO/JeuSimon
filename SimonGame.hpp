#ifndef SIMON_GAME_H
#define SIMON_GAME_H

#include "mbed.h"
#include <string>
#include "ThisThread.h"

const int MAX_LEVEL = 100; // Niveau maximum du jeu mis en variable globale

// Classe du jeu Simon
class SimonGame {
public: // Les méthodes publiques
    SimonGame();
    void loop();
    
private:    // Les méthodes privées
    int sequence[MAX_LEVEL];
    int your_sequence[MAX_LEVEL];
    int level;
    int velocity;

    // Les pins
    DigitalOut redLED;
    DigitalOut greenLED;
    DigitalOut blueLED;
    DigitalIn startButton;
    AnalogIn analogInput;
    
    float buttonValues[5];  // Valeurs des boutons

    // Les méthodes
    void setup();
    void show_sequence();
    void get_sequence();
    void generate_sequence();
    void wrong_sequence();
    void right_sequence();
    int detect_button(float inputValue);
};

#endif // SIMON_GAME_H
