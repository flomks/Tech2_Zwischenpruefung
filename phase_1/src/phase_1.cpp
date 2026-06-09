#include <Arduino.h>

// Pins
const int A_R = 18;
const int A_Y = 19;
const int A_G = 21;

const unsigned long GREEN_DURATION = 5000;
const unsigned long YELLOW_DURATION = 3000;
const unsigned long RED_YELLOW_DURATION = 2000;
const unsigned long RED_DURATION = 7000;

enum State
{
    A_RED,
    A_RED_YELLOW,
    A_GREEN,
    A_YELLOW
};

State currentState = A_RED;
unsigned long lastStateChange = 0;

void allLedsOff()
{
    digitalWrite(A_R, LOW);
    digitalWrite(A_Y, LOW);
    digitalWrite(A_G, LOW);
}

void currentLedOn()
{

    allLedsOff();

    switch (currentState)
    {
    case A_RED:
        digitalWrite(A_R, HIGH);
        break;
    case A_RED_YELLOW:
        digitalWrite(A_R, HIGH);
        digitalWrite(A_Y, HIGH);
        break;
    case A_GREEN:
        digitalWrite(A_G, HIGH);
        break;
    case A_YELLOW:
        digitalWrite(A_Y, HIGH);
        break;
    }
}

unsigned long getCurrentStateDuration()
{
    switch (currentState)
    {
    case A_RED:
        return RED_DURATION;

    case A_RED_YELLOW:
        return RED_YELLOW_DURATION;

    case A_GREEN:
        return GREEN_DURATION;

    case A_YELLOW:
        return YELLOW_DURATION;
    }

    return 0;
}

void nextState()
{
    switch (currentState)
    {
    case A_RED:
        currentState = A_RED_YELLOW;
        break;
    case A_RED_YELLOW:
        currentState = A_GREEN;
        break;
    case A_GREEN:
        currentState = A_YELLOW;
        break;
    case A_YELLOW:
        currentState = A_RED;
        break;
    }
}

void phase1Setup()
{
    pinMode(A_R, OUTPUT);
    pinMode(A_Y, OUTPUT);
    pinMode(A_G, OUTPUT);

    lastStateChange = millis();
    currentLedOn();
}

void phase1Loop()
{
    if (millis() - lastStateChange >= getCurrentStateDuration())
    {
        nextState();
        currentLedOn();
        lastStateChange = millis();
    }
}