#include <Arduino.h>

// Pins
const int A_R = 18;
const int A_Y = 19;
const int A_G = 21;

const int B_R = 22;
const int B_Y = 23;
const int B_G = 14;

const unsigned long GREEN_DURATION = 5000;
const unsigned long YELLOW_DURATION = 3000;
const unsigned long RED_YELLOW_DURATION = 3000;

const unsigned long ALL_RED_DURATION = 5000;

bool isB = false;
unsigned long lastStateChange = 0;

enum State
{
    ALL_RED_A,
    A_RED_YELLOW,
    A_GREEN,
    A_YELLOW,

    ALL_RED_B,
    B_RED_YELLOW,
    B_GREEN,
    B_YELLOW,
};

State currentState = ALL_RED_A;

void allLedsOff()
{
    digitalWrite(A_R, LOW);
    digitalWrite(A_Y, LOW);
    digitalWrite(A_G, LOW);

    digitalWrite(B_R, LOW);
    digitalWrite(B_Y, LOW);
    digitalWrite(B_G, LOW);
}

void currentLedOn()
{
    allLedsOff();

    switch (currentState)
    {

    case ALL_RED_A:
        digitalWrite(A_R, HIGH);
        digitalWrite(B_R, HIGH);
        break;

    case ALL_RED_B:
        digitalWrite(A_R, HIGH);
        digitalWrite(B_R, HIGH);
        break;

    case A_RED_YELLOW:
        digitalWrite(A_R, HIGH);
        digitalWrite(A_Y, HIGH);

        digitalWrite(B_R, HIGH);
        break;

    case A_GREEN:
        digitalWrite(A_G, HIGH);

        digitalWrite(B_R, HIGH);
        break;

    case A_YELLOW:
        digitalWrite(A_Y, HIGH);
        digitalWrite(B_R, HIGH);
        break;

    case B_RED_YELLOW:
        digitalWrite(A_R, HIGH);

        digitalWrite(B_R, HIGH);
        digitalWrite(B_Y, HIGH);
        break;

    case B_GREEN:
        digitalWrite(A_R, HIGH);

        digitalWrite(B_G, HIGH);
        break;

    case B_YELLOW:
        digitalWrite(A_R, HIGH);

        digitalWrite(B_Y, HIGH);
        break;
    }
}

unsigned long getCurrentStateDuration()
{
    switch (currentState)
    {
    case ALL_RED_A:
        return ALL_RED_DURATION;

    case A_RED_YELLOW:
        return RED_YELLOW_DURATION;

    case A_GREEN:
        return GREEN_DURATION;

    case A_YELLOW:
        return YELLOW_DURATION;

    case ALL_RED_B:
        return ALL_RED_DURATION;

    case B_RED_YELLOW:
        return RED_YELLOW_DURATION;

    case B_GREEN:
        return GREEN_DURATION;

    case B_YELLOW:
        return YELLOW_DURATION;
    }

    return 0;
}

void nextState()
{
    switch (currentState)
    {

    case ALL_RED_A:
        currentState = A_RED_YELLOW;
        break;

    case A_RED_YELLOW:
        currentState = A_GREEN;
        break;

    case A_GREEN:
        currentState = A_YELLOW;
        break;

    case A_YELLOW:
        currentState = ALL_RED_B;
        break;

    case ALL_RED_B:
        currentState = B_RED_YELLOW;
        break;

    case B_RED_YELLOW:
        currentState = B_GREEN;
        break;

    case B_GREEN:
        currentState = B_YELLOW;
        break;

    case B_YELLOW:
        currentState = ALL_RED_A;
        break;
    }
}

void phase5Setup()
{
    pinMode(A_R, OUTPUT);
    pinMode(A_Y, OUTPUT);
    pinMode(A_G, OUTPUT);

    pinMode(B_R, OUTPUT);
    pinMode(B_Y, OUTPUT);
    pinMode(B_G, OUTPUT);

    currentLedOn();
}

void phase5Loop()
{
    if (millis() - lastStateChange >= getCurrentStateDuration())
    {
        nextState();
        currentLedOn();
        lastStateChange = millis();
    }
}
