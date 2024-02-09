/*
Author : Pierrick Ledys et Yaté Mouhamadou
Communication CAN : Dimitri Britan
*/
#include "SimonGame.hpp"
#include <cstdlib>
#include "platform/mbed_thread.h"

// const int MAX_LEVEL = 100; // Niveau maximum du jeu mis en variable globale

// Classe du jeu Simon
SimonGame::SimonGame() : 
    redLED(p21),
    greenLED(p22),
    blueLED(p23),
    startButton(p9),
    analogInput(A0),
    level(1),
    velocity(1000) {
    buttonValues[0] = 600;
    buttonValues[1] = 650;
    buttonValues[2] = 700;
    buttonValues[3] = 800;
    buttonValues[4] = 900;

    setup();
}

// Les méthodes publiques
void SimonGame::loop() {
    if (level == 1)
        generate_sequence(); // générer une séquence;

    if (startButton == 0 || level != 1) // Si le bouton de démarrage est pressé ou nous gagnons
    {
        show_sequence(); // montrer la séquence
        get_sequence();  // attendre notre séquence
    }
}

// Les méthodes privées
void SimonGame::setup() {
}

// Afficher la séquence
void SimonGame::show_sequence() {
    redLED = 0;
    greenLED = 0;
    blueLED = 0;

    for (int i = 0; i < level; i++) {
        switch (sequence[i]) {
        case 2:
            redLED = 1;
            break;
        case 3:
            greenLED = 1;
            break;
        case 4:
            blueLED = 1;
            break;
        case 5:
            redLED = 1;
            greenLED = 1;
            break;
// Ajoutez des cas supplémentaires ?
        thread_sleep_for(velocity); 
        redLED = 0;
        greenLED = 0;
        blueLED = 0;
        ThisThread::sleep_for(200ms);
        }
    }
}

// Attendre notre séquence
void SimonGame::get_sequence() {
    int flag = 0; // indique si la séquence est correcte

    // Attendre que l'utilisateur appuie sur les boutons
    for (int i = 0; i < level; i++) {
        flag = 0;
        while (flag == 0) {
            // Lire l'entrée analogique et déterminer le bouton enfoncé en fonction de sa valeur
            float inputValue = analogInput.read() * 1000; // Convertir en mV
            int pressedButton = detect_button(inputValue);

            // Si un bouton est enfoncé allumer la LED correspondante
            if (pressedButton != 0) {
// Allumer les LED correspondantes
                switch (pressedButton) {
                case 2:
                    blueLED = 1;
                    break;
                case 3:
                    greenLED = 1;
                    break;
                case 4:
                    redLED = 1;
                    break;
                case 5:
                    redLED = 1;
                    greenLED = 1;
                    break;
// Ajoutez plus de cas si on fait avec 5 boutons
                }

                your_sequence[i] = pressedButton;
                flag = 1;
                ThisThread::sleep_for(200ms);

// Vérifier si le bouton enfoncé correspond à la séquence
                if (your_sequence[i] != sequence[i]) {
                    wrong_sequence();
                    return;
                }

                // Éteindre les LED
                redLED = 0;
                greenLED = 0;
                blueLED = 0;
            }
        }
    }
    right_sequence();   // Si la séquence est correcte, on continue
}

// Générer une séquence aléatoire
void SimonGame::generate_sequence() {
    srand(time(NULL)); // de cette façon, c'est vraiment aléatoire

    // Générer une séquence aléatoire
    for (int i = 0; i < MAX_LEVEL; i++) {
        sequence[i] = rand() % 4 + 2; // Ajustez en fonction de vos mappings de boutons
    }
}

// Séquence incorrecte
void SimonGame::wrong_sequence() {
// Clignoter la LED RGB avec une couleur rouge pour indiquer une mauvaise séquence
    redLED = 1;
    ThisThread::sleep_for(500ms);
    redLED = 0;

    level = 1;
    velocity = 1000;
}

// Séquence correcte
void SimonGame::right_sequence() {
// Clignoter la LED RGB avec une couleur verte pour indiquer une séquence correcte
    greenLED = 1;
    ThisThread::sleep_for(500ms);
    greenLED = 0;

    if (level < MAX_LEVEL)  // Si nous n'avons pas encore atteint le niveau maximum
        level++;

    velocity -= 50; // augmenter la difficulté
}

// Fonction pour déterminer quel bouton est enfoncé en fonction de la valeur d'entrée analogique
int SimonGame::detect_button(float inputValue) {
    for (int i = 0; i < sizeof(buttonValues) / sizeof(buttonValues[0]); i++) {
        if (inputValue > buttonValues[i] - 50 && inputValue < buttonValues[i] + 50) {
            return i + 2; // Retourne le numéro du bouton (2, 3, 4, etc.)
        }
    }
    return 0; // Aucun bouton détecté
}
