#include <Arduino.h>

// Pins
const int AA_R = 18;
const int AA_Y = 19;
const int AA_G = 21;

const int BB_R = 22;
const int BB_Y = 23;
const int BB_G = 14;

const unsigned long GREEN_DURATION2 = 5000;
const unsigned long YELLOW_DURATION2 = 3000;
const unsigned long RED_YELLOW_DURATION2 = 3000;

const unsigned long ALL_RED_DURATION2 = 5000;

// bool isB = false;
unsigned long lastStateChange2 = 0;

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

State currentState2 = ALL_RED_A;

struct States
{
    bool A_red;
    bool A_yellow;
    bool A_green;

    bool B_red;
    bool B_yellow;
    bool B_green;

    unsigned long duration;
    State nextState;
};

/*
    ALL_RED_A,
    A_RED_YELLOW,
    A_GREEN,
    A_YELLOW,

    ALL_RED_B,
    B_RED_YELLOW,
    B_GREEN,
    B_YELLOW,

*/

States allStates[] = {
    {true, false, false, true, false, false, ALL_RED_DURATION2, A_RED_YELLOW},
    {true, true, false, true, false, false, RED_YELLOW_DURATION2, A_GREEN},
    {false, false, true, true, false, false, GREEN_DURATION2, A_YELLOW},
    {true, false, false, true, false, false, YELLOW_DURATION2, ALL_RED_B},
    {true, false, false, true, false, false, ALL_RED_DURATION2, B_RED_YELLOW},
    {true, false, false, true, true, false, RED_YELLOW_DURATION2, B_GREEN},
    {true, false, false, false, false, true, GREEN_DURATION2, B_YELLOW},
    {true, false, false, true, false, false, YELLOW_DURATION2, ALL_RED_A}};

void currentLedOn2()
{
    States current = allStates[currentState2];

    digitalWrite(AA_R, current.A_red ? HIGH : LOW);
    digitalWrite(AA_Y, current.A_yellow ? HIGH : LOW);
    digitalWrite(AA_G, current.A_green ? HIGH : LOW);

    digitalWrite(BB_R, current.B_red ? HIGH : LOW);
    digitalWrite(BB_Y, current.B_yellow ? HIGH : LOW);
    digitalWrite(BB_G, current.B_green ? HIGH : LOW);
}

unsigned long getCurrentStateDuration2()
{
    return allStates[currentState2].duration;
}

void nextState2()
{
    currentState2 = allStates[currentState2].nextState;
}

void phase5v2Setup()
{
    pinMode(AA_R, OUTPUT);
    pinMode(AA_Y, OUTPUT);
    pinMode(AA_G, OUTPUT);

    pinMode(BB_R, OUTPUT);
    pinMode(BB_Y, OUTPUT);
    pinMode(BB_G, OUTPUT);

    currentLedOn2();
}

void phase5v2Loop()
{
    if (millis() - lastStateChange2 >= getCurrentStateDuration2())
    {
        nextState2();
        currentLedOn2();
        lastStateChange2 = millis();
    }
}
