#include <Arduino.h>
// Automatic Water Tank Filling System

// *** NOTE ***
// All the relay pins are considered ACTIVE HIGH here !!!
#define SOLENOID_VALVE_1_PIN 4  // Pin for solenoid valve 1
#define SOLENOID_VALVE_2_PIN 5  // Pin for solenoid valve 2
#define SOLENOID_VALVE_3_PIN A4 // Pin for solenoid valve 2
#define SOLENOID_VALVE_4_PIN A5 // Pin for solenoid valve 2
#define WATER_PUMP_PIN 6        // Pin for water pump

// Shift Register Pins 74HC165
#define latchPin 10
#define clockPin 9
#define dataPin 8
#define pulseWidth 10 // pulse width in microseconds
uint16_t buttonState = 0;

// ******************** Shift Register Pins 0 to 15 ********************
// water level indicator TANK 1
#define TANK_1_20_PIN 0  // Pin for tank 1 20% sensor
#define TANK_1_50_PIN 1  // Pin for tank 1 50% sensor
#define TANK_1_100_PIN 2 // Pin for tank 1 100% sensor

// water level indicator TANK 2
#define TANK_2_20_PIN 3  // Pin for tank 2 20% sensor
#define TANK_2_50_PIN 4  // Pin for tank 2 50% sensor
#define TANK_2_100_PIN 5 // Pin for tank 2 100% sensor

// water level indicator TANK 3
#define TANK_3_20_PIN 6  // Pin for tank 3 20% sensor
#define TANK_3_50_PIN 7  // Pin for tank 3 50% sensor
#define TANK_3_100_PIN 8 // Pin for tank 3 100% sensor

// water level indicator TANK 4
#define TANK_4_20_PIN 9   // Pin for tank 4 20% sensor
#define TANK_4_50_PIN 10  // Pin for tank 4 50% sensor
#define TANK_4_100_PIN 11 // Pin for tank 4 100% sensor

// ***************** Shift Register Pins 0 to 15 --end *****************

// Interrupt Pins
#define onIntrPin 2
#define offIntrPin 3

// Tank 1 Water level status
bool tank_1_20 = false;
bool tank_1_50 = false;
bool tank_1_100 = false;

// Tank 2 Water level status
bool tank_2_20 = false;
bool tank_2_50 = false;
bool tank_2_100 = false;

// Tank 3 Water level status
bool tank_3_20 = false;
bool tank_3_50 = false;
bool tank_3_100 = false;

// Tank 4 Water level status
bool tank_4_20 = false;
bool tank_4_50 = false;
bool tank_4_100 = false;

// Solanoid Valve status
bool solenoid_valve_1_on = false;
bool solenoid_valve_2_on = false;
bool solenoid_valve_3_on = false;
bool solenoid_valve_4_on = false;

// Water pump status
bool water_pump_on = false;

// Interrupt Pins
bool led_state = HIGH;
bool Loop_Status = true;

void setup()
{
    pinMode(SOLENOID_VALVE_1_PIN, OUTPUT);
    pinMode(SOLENOID_VALVE_2_PIN, OUTPUT);
    pinMode(SOLENOID_VALVE_3_PIN, OUTPUT);
    pinMode(SOLENOID_VALVE_4_PIN, OUTPUT);
    pinMode(WATER_PUMP_PIN, OUTPUT);

    // Shift Register Setup
    pinMode(clockPin, OUTPUT); // clock signal, idle LOW
    pinMode(latchPin, OUTPUT); // latch (copy input into registers), idle HIGH
    digitalWrite(latchPin, HIGH);

    // INTERRUPT SETUP
    pinMode(A0, OUTPUT);
    pinMode(onIntrPin, INPUT_PULLUP);
    pinMode(offIntrPin, INPUT_PULLUP);
    digitalWrite(A0, led_state);
    attachInterrupt(digitalPinToInterrupt(onIntrPin), START_ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(offIntrPin), STOP_ISR, FALLING);
}

void loop()
{
    if (Loop_Status)
    {
        // Check for current water level
        checkWaterLevelStatus();

        // Tank Logic
        tank_1_Logic();
        tank_2_Logic();
        tank_3_Logic();
        tank_4_Logic();

        // Pump Logic
        pumpLogic();

        // Updating pin Status
        updateRelayStatus();
    }
    else
    {
        solenoid_valve_1_on = false;
        solenoid_valve_2_on = false;
        solenoid_valve_3_on = false;
        solenoid_valve_4_on = false;
        water_pump_on = false;
        updateRelayStatus();
    }

    // delay(10000);
}

void pumpLogic()
{
    // Water pump logic
    if ((solenoid_valve_1_on || solenoid_valve_2_on || solenoid_valve_3_on || solenoid_valve_4_on) && !water_pump_on)
    {
        // Turn on water pump if any solenoid valve is on and pump is not already on
        water_pump_on = true;
    }
    else if (!(solenoid_valve_1_on || solenoid_valve_2_on || solenoid_valve_3_on || solenoid_valve_4_on) && water_pump_on)
    {
        // Turn off water pump if no solenoid valve is on and pump is already on
        water_pump_on = false;
    }
}

void tank_1_Logic()
{
    // Tank 1 logic
    if (!tank_1_100)
    { // Tank is not full
        if (!tank_1_50)
        {
            if (!solenoid_valve_1_on && !tank_1_20)
            {
                // Turn on solenoid valve 1 if tank 1 is 20% or below
                solenoid_valve_1_on = true;
            }
        }
    }
    else if (solenoid_valve_1_on && tank_1_100)
    {
        // Turn off solenoid valve 1 if tank 1 is 100%
        solenoid_valve_1_on = false;
    }
}

void tank_2_Logic()
{
    // Tank 2 logic
    if (!tank_2_100)
    { // Tank is not full
        if (!tank_2_50)
        {
            if (!solenoid_valve_2_on && !tank_2_20)
            {
                // Turn on solenoid valve 2 if tank 2 is 20% or below
                solenoid_valve_2_on = true;
            }
        }
    }
    else if (solenoid_valve_2_on && tank_2_100)
    {
        // Turn off solenoid valve 2 if tank 2 is 100%
        solenoid_valve_2_on = false;
    }
}

void tank_3_Logic()
{
    // Tank 3 logic
    if (!tank_3_100)
    { // Tank is not full
        if (!tank_3_50)
        {
            if (!solenoid_valve_3_on && !tank_3_20)
            {
                // Turn on solenoid valve 3 if tank 3 is 20% or below
                solenoid_valve_3_on = true;
            }
        }
    }
    else if (solenoid_valve_3_on && tank_3_100)
    {
        // Turn off solenoid valve 3 if tank 3 is 100%
        solenoid_valve_3_on = false;
    }
}

void tank_4_Logic()
{
    // Tank 4 logic
    if (!tank_4_100)
    { // Tank is not full
        if (!tank_4_50)
        {
            if (!solenoid_valve_4_on && !tank_4_20)
            {
                // Turn on solenoid valve 4 if tank 4 is 20% or below
                solenoid_valve_4_on = true;
            }
        }
    }
    else if (solenoid_valve_4_on && tank_4_100)
    {
        // Turn off solenoid valve 4 if tank 4 is 100%
        solenoid_valve_4_on = false;
    }
}

void checkWaterLevelStatus()
{
    read_state();
    // Since all water level indicator pins are pilled up
    // Hence their values are inverted !!!

    // Read tank 1 sensors
    tank_1_20 = bitRead(buttonState, TANK_1_20_PIN);
    tank_1_50 = bitRead(buttonState, TANK_1_50_PIN);
    tank_1_100 = bitRead(buttonState, TANK_1_100_PIN);

    // Read tank 2 sensors
    tank_2_20 = bitRead(buttonState, TANK_2_20_PIN);
    tank_2_50 = bitRead(buttonState, TANK_2_50_PIN);
    tank_2_100 = bitRead(buttonState, TANK_2_100_PIN);

    // Read tank 3 sensors
    tank_3_20 = bitRead(buttonState, TANK_3_20_PIN);
    tank_3_50 = bitRead(buttonState, TANK_3_50_PIN);
    tank_3_100 = bitRead(buttonState, TANK_3_100_PIN);

    // Read tank 4 sensors
    tank_4_20 = bitRead(buttonState, TANK_4_20_PIN);
    tank_4_50 = bitRead(buttonState, TANK_4_50_PIN);
    tank_4_100 = bitRead(buttonState, TANK_4_100_PIN);
}

// It is set up for ACTIVE HIGH devices
void updateRelayStatus()
{
    // USE `` ! `` WHEN USING "' ACTIVE LOW '" DEVICES
    digitalWrite(SOLENOID_VALVE_1_PIN, solenoid_valve_1_on);
    digitalWrite(SOLENOID_VALVE_2_PIN, solenoid_valve_2_on);
    digitalWrite(SOLENOID_VALVE_3_PIN, solenoid_valve_3_on);
    digitalWrite(SOLENOID_VALVE_4_PIN, solenoid_valve_4_on);
    digitalWrite(WATER_PUMP_PIN, water_pump_on);
}

// ************************* Shift Register Func. *************************

void read_state()
{
    // Give a pulse to the parallel load latch of all 74HC165
    buttonState = 0;

    digitalWrite(latchPin, LOW);
    delayMicroseconds(pulseWidth);
    digitalWrite(latchPin, HIGH);

    for (int i = 8; i >= 0; i -= 8)
    {
        buttonState |= ((uint16_t)ReadOne165()) << i;
    }
}

byte ReadOne165()
{

    byte ret = 0x00;

    // The first one that is read is the highest bit (input D7 of the 74HC165).
    for (int i = 7; i >= 0; i--)
    {
        if (digitalRead(dataPin) == HIGH)
            bitSet(ret, i);

        digitalWrite(clockPin, HIGH);
        delayMicroseconds(pulseWidth);
        digitalWrite(clockPin, LOW);
    }

    return (ret);
}

// ********************** Shift Register Func. --end ***********************

// INTERRUPT FUNCTIONS :- ISRs  (INTERRUPT SERVICE ROUTINE)s

void STOP_ISR()
{
    Loop_Status = false;

    led_state = LOW;
    digitalWrite(A0, led_state);
}

void START_ISR()
{
    Loop_Status = true;

    led_state = HIGH;
    digitalWrite(A0, led_state);
}