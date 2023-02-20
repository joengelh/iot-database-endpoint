#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Wire.h>
#include "Adafruit_SGP30.h"

const char *wifiSsid = "xxxxx";
const char *wifiPassword = "xxxxx";

// Replace with your Basic Authentication credentials
const char *baseauthUsername = "xxxxx";
const char *baseauthPassword = "xxxxx";

const char *api = "xxxxx";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;

// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 60000;

int mq138Input = A0;

Adafruit_SGP30 sgp;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    Serial.begin(115200);
    WiFi.begin(wifiSsid, wifiPassword);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    if (!sgp.begin())
    {
        Serial.println("Sensor not found :-(");
        while (1)
            ;
    }

    // Hier können die Baseline Werte wenn vorhanden eingegeben werden, wenn nicht, diese Zeile auskommentierern
    sgp.setIAQBaseline(0x8A3E, 0x9692); // Baseline Werte nach 12h Betrieb vom Serial Monitor entnehmen!

    digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
}

void loop()
{
    // Send an HTTP POST request every minute
    if ((millis() - lastTime) > timerDelay)
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)

        float Vi = 0.1;
        float Vc = 4.95;                          // Constant voltage from pin
        int sensorValue = analogRead(mq138Input); // Read analog pin for voltage info in 10 bits
        float Vf = sensorValue * 4.95 / 1023;     // Voltage through pin when in presence of VOCs (formaldehyde)
        float formaldehydeConcentration = concentrationPPMform(Vf, Vi);
        Serial.print("Vi: ");
        Serial.println(Vf);
        Serial.print("Formaldehyde Concentration: ");
        Serial.print(formaldehydeConcentration);
        Serial.print("ppm\n"); // Print concentration data

        // Hier können die Luftfeuchte und Temperaturwerte für genauere Messwerte eingegeben werden.
        float temperature = 21.1; // [°C]
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

        {
            // Check WiFi connection status
            if (WiFi.status() == WL_CONNECTED)
            {
                WiFiClient client;
                HTTPClient http;

                // Your Domain name with URL path or IP address with path
                http.begin(client, api);

                // If you need Node-RED/server authentication, insert user and password below
                // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
                http.setAuthorization(baseauthUsername, baseauthPassword);

                // If you need an HTTP request with a content type: application/json, use the following:
                http.addHeader("Content-Type", "application/json");
                int httpResponseCode = http.POST("{\"device\":\"live\",\"float1\":" + String(Vf) + ",\"float2\":" + String(formaldehydeConcentration) + ",\"float3\":" + String(sgp.TVOC) + ",\"float4\":" + String(sgp.eCO2) + "}");

                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);

                // Free resources
                http.end();
            }
            else
            {
                Serial.println("WiFi Disconnected");
            }
            lastTime = millis();
            digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
        }
    }
}

uint32_t getAbsoluteHumidity(float temperature, float humidity)
{
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                // [mg/m^3]
    return absoluteHumidityScaled;
}

float concentrationPPMform(float V, float Vstart) // Formaldehde concentration formula
{

    float con = 212.32619543773774 * exp(-(7.653015806367451 * (4.95 - V) * Vstart) / (V * (4.95 - Vstart)));

    return con;
}
