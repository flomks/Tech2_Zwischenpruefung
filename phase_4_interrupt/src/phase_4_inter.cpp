#include <Arduino.h>

// Pins
const int A_R = 18;
const int A_Y = 19;
const int A_G = 21;

const int A_BUTTON = 32;

const int A_PED_R = 25;
const int A_PED_G = 26;

const unsigned long GREEN_DURATION = 5000;
const unsigned long YELLOW_DURATION = 3000;
const unsigned long RED_YELLOW_DURATION = 3000;

// PED and TL red
const unsigned long ALL_RED_DURATION = 5000;

// PED
const unsigned long A_PED_G_DURATION = 5000;

// interrupt
const unsigned long DEAD_TIME = 300; // Prellen
volatile unsigned long lastButtonPressde = 0;

volatile bool PED_NEXT = false;          // Button pressed
volatile bool PED_BUTTON_ENABLED = true; // To make sure, that the button is only active during the car phases and not during the pedestrian phases
bool CAR_PHASE_OVER = true;              // To make sure, that there are not multiple PED Phases in a row without a car phase in between

unsigned long lastStateChange = 0; // When the last State changed

void IRAM_ATTR buttonInterrupt()
{
    if (PED_BUTTON_ENABLED && (millis() - lastButtonPressde) > DEAD_TIME)
    {
        PED_NEXT = true;
        lastButtonPressde = millis();
    }
}

enum State
{
    // A_RED,
    A_RED_YELLOW,
    A_GREEN,
    A_YELLOW,

    // TL_A_RED,
    A_PED_Green,

    ALL_RED
};

// State currentState = A_RED;
State currentState = ALL_RED;

void allLedsOff()
{
    digitalWrite(A_R, LOW);
    digitalWrite(A_Y, LOW);
    digitalWrite(A_G, LOW);

    digitalWrite(A_PED_R, LOW);
    digitalWrite(A_PED_G, LOW);
}

void currentLedOn()
{
    allLedsOff();

    switch (currentState)
    {
        /*
        case A_RED:
            digitalWrite(A_R, HIGH);
            digitalWrite(A_PED_R, HIGH);
            break;
        */

    case ALL_RED:
        digitalWrite(A_R, HIGH);
        digitalWrite(A_PED_R, HIGH);
        break;

    case A_RED_YELLOW:
        digitalWrite(A_R, HIGH);
        digitalWrite(A_Y, HIGH);
        digitalWrite(A_PED_R, HIGH);
        break;

    case A_GREEN:
        digitalWrite(A_G, HIGH);
        digitalWrite(A_PED_R, HIGH);
        break;

    case A_YELLOW:
        digitalWrite(A_Y, HIGH);
        digitalWrite(A_PED_R, HIGH);
        break;

        /*
        case TL_A_RED:
            digitalWrite(A_R, HIGH);
            digitalWrite(TL_A_R, HIGH);
            break;
        */

    case A_PED_Green:
        digitalWrite(A_R, HIGH);
        digitalWrite(A_PED_G, HIGH);
        break;
    }
}

unsigned long getCurrentStateDuration()
{
    switch (currentState)
    {

        /*
        case A_RED:
            return RED_DURATION;
        */

    case ALL_RED:
        return ALL_RED_DURATION;

    case A_RED_YELLOW:
        return RED_YELLOW_DURATION;

    case A_GREEN:
        return GREEN_DURATION;

    case A_YELLOW:
        return YELLOW_DURATION;

        /*
        case TL_A_RED:
            return TL_A_R_DURATION;
        */

    case A_PED_Green:
        return A_PED_G_DURATION;
    }

    return 0;
}

void nextState()
{
    switch (currentState)
    {

        /*
        case A_RED:
            currentState = A_RED_YELLOW;
            break;
        */

    case ALL_RED:
        if (PED_NEXT && CAR_PHASE_OVER)
        {
            currentState = A_PED_Green;
            PED_NEXT = false;
            PED_BUTTON_ENABLED = false;
            CAR_PHASE_OVER = false;
        }
        else
        {
            currentState = A_RED_YELLOW;
            PED_BUTTON_ENABLED = true;
        }
        break;

    case A_RED_YELLOW:
        currentState = A_GREEN;
        break;

    case A_GREEN:
        if (PED_NEXT)
        {
            currentState = A_YELLOW;
            CAR_PHASE_OVER = true;
        }

        break;

    case A_YELLOW:
        currentState = ALL_RED;
        PED_BUTTON_ENABLED = true;
        break;

        /*
    case A_PED_R:
        currentState = A_PED_Green;
        break;
        */

    case A_PED_Green:
        currentState = ALL_RED;
        PED_BUTTON_ENABLED = true;
        break;
    }
}

void phase4Setup()
{
    pinMode(A_R, OUTPUT);
    pinMode(A_Y, OUTPUT);
    pinMode(A_G, OUTPUT);

    pinMode(A_PED_R, OUTPUT);
    pinMode(A_PED_G, OUTPUT);

    pinMode(A_BUTTON, INPUT_PULLUP);

    // Interrupt regisitrieren. FALLING weil es PULLUP ist -> HIGH auf LOW.
    // Bei einem PULLDOWN -> LOWauf HIGH wäre es RISING
    attachInterrupt(digitalPinToInterrupt(A_BUTTON), buttonInterrupt, FALLING);

    lastStateChange = millis();
    currentLedOn();
}

void phase4Loop()
{
    if (millis() - lastStateChange >= getCurrentStateDuration())
    {
        nextState();
        currentLedOn();
        lastStateChange = millis();
    }
}
