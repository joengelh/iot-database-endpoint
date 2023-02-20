#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

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
unsigned long timerDelay = 5000;

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
    digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    // Send an HTTP POST request every 10 minutes
    if ((millis() - lastTime) > timerDelay)
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
            int httpResponseCode = http.POST("{\"device\":\"iot0\",\"float1\":0.2,\"float2\":0.2,\"float3\":0.2,\"float4\":0.2}");

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
    }
    digitalWrite(LED_BUILTIN, LOW); // turn the LED on (HIGH is the voltage level)
}
