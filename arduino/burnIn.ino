// BURN in procedure

#include <Wire.h>
#include "Adafruit_SGP30.h"

// For reliable measurements, it is necessary to stabilize the sensor for 2 hours //
// To do this, the system should be connected to the power supply socket while the sensor is cured for this period of time without exposure to the sample //

int mq138Input = A0;

// Declare variables for stability measurement of the MQ-138 sensor:

int delta = 0;
int oldValues = 0;
int steps = 0;

Adafruit_SGP30 sgp;

int counter = 0;
const int sensorTakt = 1000; // jede Sekunde wird der Sensor ausgelesen

uint32_t getAbsoluteHumidity(float temperature, float humidity)
{
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                // [mg/m^3]
    return absoluteHumidityScaled;
}

void setup()
{
    Serial.begin(115200);

    pinMode(mq138Input, INPUT); // Make the MQ-138's pin an input

    if (!sgp.begin())
    {
        Serial.println("Sensor not found :-(");
        while (1)
            ;
    }
    // Hier können die Baseline Werte wenn vorhanden eingegeben werden, wenn nicht, diese Zeile auskommentierern
    // sgp.setIAQBaseline(0x7E94, 0x8A51);  // Baseline Werte nach 12h Betrieb vom Serial Monitor entnehmen!
}

void loop()
{

    float Vi = 0.1;

    float Vc = 4.95; // Constant voltage from pin

    int sensorValue = analogRead(mq138Input); // Read analog pin for voltage info in 10 bits

    float Vf = sensorValue * 4.95 / 1023; // Voltage through pin when in presence of VOCs (formaldehyde)

    Serial.print("Vi: ");

    Serial.println(sensorValue * 4.95 / 1023);

    Serial.print("Formaldehyde Concentration: ");

    Serial.print(concentrationPPMform(Vf, Vi));

    Serial.print("ppm\n"); // Print concentration data

    // Hier können die Luftfeuchte und Temperaturwerte für genauere Messwerte eingegeben werden.
    float temperature = 22.1; // [°C]
    float humidity = 50;      // [%RH]
    sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

    if (!sgp.IAQmeasure())
    {
        Serial.println("Measurement failed");
        return;
    }
    Serial.print("TVOC ");
    Serial.print(sgp.TVOC);
    Serial.print(" ppb\t");
    Serial.print("eCO2 ");
    Serial.print(sgp.eCO2);
    Serial.println(" ppm");

    delay(sensorTakt);

    counter++;
    if (counter == 30)
    {
        counter = 0;
        uint16_t TVOC_base, eCO2_base;
        if (!sgp.getIAQBaseline(&eCO2_base, &TVOC_base))
        {
            Serial.println("Failed to get baseline readings");
            return;
        }
        Serial.print("Baseline Werte: eCO2: 0x");
        Serial.print(eCO2_base, HEX);
        Serial.print(" & TVOC: 0x");
        Serial.println(TVOC_base, HEX);
    }
}

float concentrationPPMform(float V, float Vstart) // Formaldehde concentration formula

{

    float con = 212.32619543773774 * exp(-(7.653015806367451 * (4.95 - V) * Vstart) / (V * (4.95 - Vstart)));

    return con;
}
